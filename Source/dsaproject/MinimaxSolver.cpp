#include "MinimaxSolver.h"
#include "Kismet/KismetMathLibrary.h"

namespace BattleConstants
{
    const int32 BaseDamage = 20;
    const float ChargeMultiplier = 1.5f;
    const int32 MaxSearchDepth = 3;
}

EBattleAction FMinimaxSolver::FindBestAction(const FGameState& CurrentState)
{
    float BestScore = -FLT_MAX;
    EBattleAction BestAction = EBattleAction::Attack;

    TArray<EBattleAction> PossibleActions = { EBattleAction::Attack, EBattleAction::Defend, EBattleAction::Charge };

    for (EBattleAction Action : PossibleActions)
    {
        // Simulate Turn will apply the damage logic using the CURRENT state flags
        FGameState NextState = SimulateTurn(CurrentState, Action, true);
        float Score = Minimax(NextState, BattleConstants::MaxSearchDepth - 1, false);

        if (Score > BestScore)
        {
            BestScore = Score;
            BestAction = Action;
        }
    }
    return BestAction;
}

float FMinimaxSolver::Minimax(FGameState State, int32 Depth, bool bIsBossTurn)
{
    if (Depth == 0 || State.PlayerHP <= 0 || State.BossHP <= 0)
    {
        return EvaluateState(State);
    }

    TArray<EBattleAction> PossibleActions = { EBattleAction::Attack, EBattleAction::Defend, EBattleAction::Charge };

    if (bIsBossTurn)
    {
        float MaxScore = -FLT_MAX;
        for (EBattleAction Action : PossibleActions)
        {
            FGameState NextState = SimulateTurn(State, Action, true);
            MaxScore = FMath::Max(MaxScore, Minimax(NextState, Depth - 1, false));
        }
        return MaxScore;
    }
    else
    {
        float MinScore = FLT_MAX;
        for (EBattleAction Action : PossibleActions)
        {
            FGameState NextState = SimulateTurn(State, Action, false);
            MinScore = FMath::Min(MinScore, Minimax(NextState, Depth - 1, true));
        }
        return MinScore;
    }
}

FGameState FMinimaxSolver::SimulateTurn(FGameState State, EBattleAction Action, bool bIsBossActor)
{
    FGameState NewState = State;

    int32* TargetHP = bIsBossActor ? &NewState.PlayerHP : &NewState.BossHP;
    bool* ActorDefending = bIsBossActor ? &NewState.bBossDefending : &NewState.bPlayerDefending;
    bool* ActorCharged = bIsBossActor ? &NewState.bBossCharged : &NewState.bPlayerCharged;

    // Capture states used for calculation BEFORE we update them for next turn
    bool bWasCharged = *ActorCharged;

    // CRITICAL: We use the Target's CURRENT Defending state (from previous turn)
    bool bTargetGuarding = bIsBossActor ? State.bPlayerDefending : State.bBossDefending;

    // Reset Actor's Defense flag immediately (Defense lasts until they act again)
    *ActorDefending = false;

    if (Action == EBattleAction::Defend)
    {
        *ActorDefending = true; // Sets defense for the NEXT time they get hit
    }
    else if (Action == EBattleAction::Charge)
    {
        *ActorCharged = true;
    }
    else if (Action == EBattleAction::Attack)
    {
        EBattleAction DefenderAction = bIsBossActor ? State.LastPlayerAction : State.LastBossAction;

        // Pass bTargetGuarding explicitly
        int32 Damage = CalculateDamage(Action, DefenderAction, !bIsBossActor, bWasCharged, bTargetGuarding);

        *TargetHP -= Damage;
        *ActorCharged = false;
    }

    if (bIsBossActor) NewState.LastBossAction = Action;
    else NewState.LastPlayerAction = Action;

    return NewState;
}

int32 FMinimaxSolver::CalculateDamage(EBattleAction AttackerAction, EBattleAction DefenderAction, bool bIsPlayerAttacking, bool bIsAttackerCharged, bool bIsDefenderGuarding)
{
    float Damage = 0.0f;

    // 1. ATTACK LOGIC
    if (AttackerAction == EBattleAction::Attack)
    {
        Damage = BattleConstants::BaseDamage;

        if (bIsAttackerCharged)
        {
            Damage *= BattleConstants::ChargeMultiplier;
        }

        // 2. DEFENSE CHECK
        // We now check the State (bIsDefenderGuarding), NOT the Action.
        // Even if DefenderAction == Defend (chosen this turn), bIsDefenderGuarding might be false (wasn't defending last turn).
        if (bIsDefenderGuarding)
        {
            // Shield Break Logic: Charge breaks shield, Normal attack gets blocked
            if (!bIsAttackerCharged)
            {
                Damage = 0.0f;
            }
        }
    }
    else
    {
        return 0; // Not attacking
    }

    if (!bIsPlayerAttacking) Damage *= 0.8f;

    return FMath::Max(0, FMath::RoundToInt(Damage));
}

float FMinimaxSolver::EvaluateState(const FGameState& State)
{
    int32 Score = (State.BossHP - State.PlayerHP);
    if (State.bBossCharged) Score += 15;
    if (State.PlayerHP <= 0) return 10000.0f;
    if (State.BossHP <= 0) return -10000.0f;
    return (float)Score;
}