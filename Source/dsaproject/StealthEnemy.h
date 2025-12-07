#pragma once

#include "CoreMinimal.h"
#include "EnemyMonster.h" // Inherit from Base
#include "StealthEnemy.generated.h"

/**
 * AStealthEnemy
 * Inherits Patrol behavior from AEnemyMonster.
 * Adds: Cone of Vision logic using Dot Product and Raycasting.
 */
UCLASS()
class DSAPROJECT_API AStealthEnemy : public AEnemyMonster
{
    GENERATED_BODY()

public:
    AStealthEnemy();
    virtual void Tick(float DeltaTime) override;

protected:
    /** How far the enemy can see (e.g., 500 units) */
    UPROPERTY(EditAnywhere, Category = "AI|Stealth")
        float SightRange;

    /** Field of View in Degrees (e.g., 90 degrees) */
    UPROPERTY(EditAnywhere, Category = "AI|Stealth")
        float VisionAngle;

    /** DEBUG: Show the vision cone lines in game? */
    UPROPERTY(EditAnywhere, Category = "AI|Stealth")
        bool bShowDebugVision;

private:
    /** ALGORITHM: Checks Dot Product & Line of Sight */
    bool CanSeePlayer(AActor* Player);

    /** Logic when player is caught */
    void GameOver();
};