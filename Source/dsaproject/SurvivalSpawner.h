#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyMonster.h"
#include "Containers/Queue.h" // Data Structure: Queue
#include "SurvivalSpawner.generated.h"

UCLASS()
class DSAPROJECT_API ASurvivalSpawner : public AActor
{
    GENERATED_BODY()

public:
    ASurvivalSpawner();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    /** Which enemy class to spawn (e.g., BP_GreedyEnemy) */
    UPROPERTY(EditAnywhere, Category = "DSA|Pooling")
        TSubclassOf<AEnemyMonster> EnemyClass;

    /** How many enemies to keep in memory */
    UPROPERTY(EditAnywhere, Category = "DSA|Pooling")
        int32 PoolSize = 10;

    /** Time between spawns */
    UPROPERTY(EditAnywhere, Category = "GameLoop")
        float SpawnInterval = 3.0f;

    /** Total time to survive (seconds) */
    UPROPERTY(EditAnywhere, Category = "GameLoop")
        float TimeLimit = 60.0f;

    /** Locations where enemies can appear */
    UPROPERTY(EditInstanceOnly, Category = "GameLoop")
        TArray<AActor*> SpawnPoints;

private:
    /** DSA DATA STRUCTURE: TQueue for Object Pooling (FIFO) */
    TQueue<AEnemyMonster*> EnemyPool;

    /** Timer handles */
    FTimerHandle SpawnTimerHandle;
    float TimeElapsed;
    bool bLevelActive;

    /** ALGORITHM: Get next available object from pool */
    void ActivateNextEnemy();

    /** Win condition */
    void WinLevel();
};