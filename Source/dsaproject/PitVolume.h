#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PitVolume.generated.h"

class UBoxComponent;

UCLASS()
class DSAPROJECT_API APitVolume : public AActor
{
    GENERATED_BODY()

public:
    APitVolume();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pit")
        UBoxComponent* CollisionComp;

private:
    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};