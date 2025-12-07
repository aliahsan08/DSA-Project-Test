#include "PitVolume.h"
#include "Components/BoxComponent.h"
#include "dsaprojectCharacter.h"

APitVolume::APitVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
    RootComponent = CollisionComp;
    // Set to OverlapAll to detect player falling into the pit
    CollisionComp->SetCollisionProfileName(TEXT("OverlapAll"));
    CollisionComp->SetBoxExtent(FVector(500.0f, 50.0f, 100.0f));
    CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APitVolume::OnOverlapBegin);
}

void APitVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AdsaprojectCharacter* Player = Cast<AdsaprojectCharacter>(OtherActor);
    if (Player)
    {
        Player->DieAndRespawn();
    }
}