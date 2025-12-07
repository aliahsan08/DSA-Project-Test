#pragma once

#include "CoreMinimal.h"

// Define possible actions for both Player and Boss
UENUM(BlueprintType)
enum class EBattleAction : uint8
{
    Attack UMETA(DisplayName = "Attack"),
    Defend UMETA(DisplayName = "Defend"),
    Charge UMETA(DisplayName = "Charge"),
    None UMETA(DisplayName = "None")
};

struct FGameState
{
    int32 PlayerHP;
    int32 BossHP;
    EBattleAction LastPlayerAction;
    EBattleAction LastBossAction;
    bool bIsBossTurn;

    // Persistent states
    bool bPlayerDefending = false;
    bool bBossDefending = false;
    bool bPlayerCharged = false;
    bool bBossCharged = false;
};

class DSAPROJECT_API FMinimaxSolver
{
public:
    static EBattleAction FindBestAction(const FGameState& CurrentState);

    // FIX: Added 'bIsDefenderGuarding' to separate the Action (Event) from the State (Block)
    static int32 CalculateDamage(EBattleAction AttackerAction, EBattleAction DefenderAction, bool bIsPlayerAttacking, bool bIsAttackerCharged, bool bIsDefenderGuarding);

private:
    static float Minimax(FGameState State, int32 Depth, bool bIsMaximizingPlayer);
    static FGameState SimulateTurn(FGameState State, EBattleAction Action, bool bIsBossActor);
    static float EvaluateState(const FGameState& State);
};