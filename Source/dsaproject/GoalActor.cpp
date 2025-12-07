#include "GoalActor.h"
#include "Components/BoxComponent.h"
#include "dsaprojectCharacter.h"
#include "dsaprojectGameMode.h"
#include "Kismet/GameplayStatics.h"

AGoalActor::AGoalActor()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
    RootComponent = CollisionComp;
    CollisionComp->SetCollisionProfileName(TEXT("OverlapAll"));
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGoalActor::OnOverlapBegin);
}

void AGoalActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AdsaprojectCharacter* Player = Cast<AdsaprojectCharacter>(OtherActor);
    if (Player)
    {
        AdsaprojectGameMode* GameMode = Cast<AdsaprojectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
        if (GameMode)
        {
            GameMode->FinishLevel(Player);
        }
    }
}