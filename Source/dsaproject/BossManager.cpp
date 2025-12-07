#include "BossManager.h"
#include "BattlePlayer.h"
#include "BossPawn.h"
#include "MinimaxSolver.h"
#include "Kismet/GameplayStatics.h"

ABossManager::ABossManager()
{
    PrimaryActorTick.bCanEverTick = false;
    PlayerHP = 100;
    BossHP = 200;
    bIsBusy = false;

    bPlayerCharged = false;
    bBossCharged = false;
    bPlayerDefending = false;
    bBossDefending = false;

    InputManager = CreateDefaultSubobject<UGameInputComponent>(TEXT("InputManager"));
}

void ABossManager::BeginPlay()
{
    Super::BeginPlay();

    // 1. Find Actors
    BattlePlayerRef = Cast<ABattlePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    BossPawnRef = Cast<ABossPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossPawn::StaticClass()));

    // 2. Create UI
    if (BattleWidgetClass)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
        {
            // Set input to Game Only so Keyboard U/I/O works
            PC->SetInputMode(FInputModeGameOnly());
            PC->bShowMouseCursor = false;

            ActiveBattleWidget = CreateWidget<UBattleWidget>(PC, BattleWidgetClass);
            if (ActiveBattleWidget)
            {
                ActiveBattleWidget->AddToViewport();
                ActiveBattleWidget->UpdateHP(PlayerHP, BossHP);
            }
        }
    }

    LogToUI("BOSS BATTLE START! Press U (Attack), I (Defend), O (Charge)");
}

void ABossManager::LogToUI(const FString& Message)
{
    if (ActiveBattleWidget)
    {
        ActiveBattleWidget->UpdateLog(Message);
        ActiveBattleWidget->UpdateHP(PlayerHP, BossHP);
    }
}

void ABossManager::ExecuteTurn(EBattleAction PlayerAction)
{
    if (bIsBusy || PlayerHP <= 0 || BossHP <= 0) return;

    bIsBusy = true;
    PendingPlayerAction = PlayerAction;

    // Step 1: Player acts immediately
    Sequence_PlayerAction();
}

void ABossManager::Sequence_PlayerAction()
{
    if (BattlePlayerRef)
    {
        if (PendingPlayerAction == EBattleAction::Attack) BattlePlayerRef->PlayAttackAnim();
        else if (PendingPlayerAction == EBattleAction::Defend) BattlePlayerRef->PlayDefendAnim();
    }

    FString ActionText = *UEnum::GetValueAsString(PendingPlayerAction);
    ActionText.RemoveFromStart("EBattleAction::");
    LogToUI(FString::Printf(TEXT("You chose to %s!"), *ActionText));

    if (PendingPlayerAction == EBattleAction::Attack)
    {
        // Apply Player Damage NOW
        int32 Dmg = FMinimaxSolver::CalculateDamage(PendingPlayerAction, EBattleAction::None, true, bPlayerCharged, bBossDefending);

        if (Dmg > 0) LogToUI(FString::Printf(TEXT(" -> Boss took %d damage!"), Dmg));
        else if (bBossDefending && !bPlayerCharged) LogToUI(" -> Boss BLOCKED the attack!");

        BossHP = FMath::Max(0, BossHP - Dmg);
        if (bPlayerCharged) { bPlayerCharged = false; }
    }
    else if (PendingPlayerAction == EBattleAction::Charge)
    {
        LogToUI(" -> You assume a charging stance!");
    }
    else if (PendingPlayerAction == EBattleAction::Defend)
    {
        LogToUI(" -> You raise your guard!");
    }

    if (BossHP <= 0)
    {
        EndBattle(true);
        return;
    }

    GetWorldTimerManager().SetTimer(BattleSequenceTimer, this, &ABossManager::Sequence_BossDecision, 2.0f, false);
}

void ABossManager::Sequence_BossDecision()
{
    FGameState CurrentState;
    CurrentState.PlayerHP = PlayerHP;
    CurrentState.BossHP = BossHP;
    CurrentState.LastPlayerAction = PendingPlayerAction;
    CurrentState.bPlayerDefending = bPlayerDefending;
    CurrentState.bBossDefending = bBossDefending;
    CurrentState.bPlayerCharged = bPlayerCharged;
    CurrentState.bBossCharged = bBossCharged;

    PendingBossAction = FMinimaxSolver::FindBestAction(CurrentState);
    LogToUI("The Boss is reacting...");

    GetWorldTimerManager().SetTimer(BattleSequenceTimer, this, &ABossManager::Sequence_BossAction, 0.5f, false);
}

void ABossManager::Sequence_BossAction()
{
    FString ActionText = *UEnum::GetValueAsString(PendingBossAction);
    ActionText.RemoveFromStart("EBattleAction::");
    LogToUI(FString::Printf(TEXT("BOSS used %s!"), *ActionText));

    if (BossPawnRef)
    {
        if (PendingBossAction == EBattleAction::Attack) BossPawnRef->PlayAttack();
        else if (PendingBossAction == EBattleAction::Defend) BossPawnRef->PlayDefend();
    }

    if (PendingBossAction == EBattleAction::Attack)
    {
        bool bPlayerIsGuardingNow = (PendingPlayerAction == EBattleAction::Defend);

        int32 Dmg = FMinimaxSolver::CalculateDamage(PendingBossAction, EBattleAction::None, false, bBossCharged, bPlayerIsGuardingNow);

        if (Dmg > 0) LogToUI(FString::Printf(TEXT(" -> You took %d damage!"), Dmg));
        else if (bPlayerIsGuardingNow && !bBossCharged) LogToUI(" -> You BLOCKED the attack!");

        PlayerHP = FMath::Max(0, PlayerHP - Dmg);
        if (bBossCharged) { bBossCharged = false; }
    }
    else if (PendingBossAction == EBattleAction::Charge)
    {
        LogToUI(" -> Boss is charging power!");
    }

    if (PlayerHP <= 0)
    {
        EndBattle(false);
        return;
    }

    GetWorldTimerManager().SetTimer(BattleSequenceTimer, this, &ABossManager::Sequence_ResolveTurn, 2.0f, false);
}

void ABossManager::Sequence_ResolveTurn()
{
    // Update Flags for NEXT turn
    bPlayerDefending = (PendingPlayerAction == EBattleAction::Defend);
    bBossDefending = (PendingBossAction == EBattleAction::Defend);

    if (PendingPlayerAction == EBattleAction::Charge) bPlayerCharged = true;
    if (PendingBossAction == EBattleAction::Charge) bBossCharged = true;

    bIsBusy = false;
    LogToUI("Your Turn...");
}

void ABossManager::EndBattle(bool bPlayerWon)
{
    if (bPlayerWon)
    {
        LogToUI("VICTORY! Boss Defeated.");
        if (BossPawnRef) BossPawnRef->PlayDeath();
    }
    else
    {
        LogToUI("DEFEAT! Try again.");
    }
}