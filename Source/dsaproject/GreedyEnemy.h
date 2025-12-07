#pragma once

#include "CoreMinimal.h"
#include "EnemyMonster.h" // Inherit from our custom Base
#include "GreedyEnemy.generated.h"

/**
 * AGreedyEnemy
 * Inherits basic movement and collision from AEnemyMonster.
 * Adds: Elliptical Vision and Greedy Best-First Search Chasing.
 */
UCLASS()
class DSAPROJECT_API AGreedyEnemy : public AEnemyMonster
{
    GENERATED_BODY()

public:
    AGreedyEnemy();
    virtual void Tick(float DeltaTime) override;

protected:
    /** Vision Range X */
    UPROPERTY(EditAnywhere, Category = "AI|Vision")
        float SightRange;

    /** Vision Range Z */
    UPROPERTY(EditAnywhere, Category = "AI|Vision")
        float SightHeight;

private:
    /** Pathfinding Data */
    TArray<FVector> PathToPlayer;
    int32 CurrentPathIndex;
    FVector TargetLocation;

    /** Advanced Logic */
    void ChasePlayer(float DeltaTime, AActor* Player);
};