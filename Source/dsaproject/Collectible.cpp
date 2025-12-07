#include "Collectible.h"
#include "Components/BoxComponent.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "dsaprojectCharacter.h"
#include "dsaprojectGameMode.h"
#include "DSAManagerComponent.h"
#include "Kismet/GameplayStatics.h"

ACollectible::ACollectible()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
    RootComponent = CollisionComp;
    CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionComp->SetGenerateOverlapEvents(true);
    CollisionComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

    SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComp"));
    SpriteComp->SetupAttachment(RootComponent);

    ItemType = ECollectibleType::ECT_Coin;
    ScoreValue = 1;
}

void ACollectible::BeginPlay()
{
    Super::BeginPlay();
    // Binding in BeginPlay is safer
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ACollectible::OnOverlapBegin);
}

void ACollectible::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if player
    AdsaprojectCharacter* Player = Cast<AdsaprojectCharacter>(OtherActor);
    if (Player)
    {
        if (Player->GetInventoryComponent())
        {
            Player->GetInventoryComponent()->AddItem(ItemType, ScoreValue);

            // DSA Queue Logic
            AdsaprojectGameMode* GameMode = Cast<AdsaprojectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
            if (GameMode && GameMode->DSAManager)
            {
                // --- FIX: Use StaticEnum here too ---
                const UEnum* EnumPtr = StaticEnum<ECollectibleType>();
                FString TypeName = EnumPtr ? EnumPtr->GetNameStringByValue((int64)ItemType) : TEXT("Unknown");

                FString Event = FString::Printf(TEXT("Collected %s (+%d)"), *TypeName, ScoreValue);
                GameMode->DSAManager->LogEvent(Event);
            }

            Destroy();
        }
    }
}