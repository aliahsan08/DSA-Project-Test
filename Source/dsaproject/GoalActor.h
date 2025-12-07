#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalActor.generated.h"

class UBoxComponent;

UCLASS()
class DSAPROJECT_API AGoalActor : public AActor
{
    GENERATED_BODY()

public:
    AGoalActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Goal")
        UBoxComponent* CollisionComp;

private:
    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};