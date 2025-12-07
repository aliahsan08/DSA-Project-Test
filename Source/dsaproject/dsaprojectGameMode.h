// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "dsaprojectGameMode.generated.h"

class AdsaprojectCharacter;
class AEnemyMonster;
class UDSAManagerComponent; // Forward declaration

/**
 * The GameMode defines the game being played.
 * It manages the game rules, scoring, and critical DSA components.
 */
 // FIX: Removed "minimalapi" from UCLASS because we are using DSAPROJECT_API
UCLASS()
class DSAPROJECT_API AdsaprojectGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AdsaprojectGameMode();

    virtual void BeginPlay() override;

    /** Player spawn location, found automatically or set in editor. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
        FVector SpawnLocation;

    /** Current score/collectible count. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Scoring")
        int32 CurrentScore;

    /** DSA: Central Manager for Tree, Stack, and Queue. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DSA")
        UDSAManagerComponent* DSAManager;

    /** DSA: TSet - Efficiently tracks active, unique enemies (O(1) lookup). */
    UPROPERTY(VisibleAnywhere, Category = "Enemies")
        TSet<AEnemyMonster*> ActiveEnemies;

    /** Called by Pit/Enemy when player dies. */
    void RespawnPlayer(AdsaprojectCharacter* DeadPlayer);

    /** Called by GoalActor when player finishes the level. */
    void FinishLevel(AdsaprojectCharacter* Player);

    /** Called by Collectible when picked up. */
    void AddScore(int32 ScoreToAdd);
};