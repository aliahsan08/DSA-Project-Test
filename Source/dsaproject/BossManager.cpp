#include "BossManager.h"
#include "BattlePlayer.h"
#include "BossPawn.h"
#include "MinimaxSolver.h"
#include "Kismet/GameplayStatics.h"
#include "DSAGameInstance.h" // Needed for Coin Count
#include "Blueprint/UserWidget.h" // For CreateWidget

ABossManager::ABossManager()
{
    PrimaryActorTick.bCanEverTick = false;
    PlayerHP = 100;
    BossHP = 200;
    bIsBusy = false;

    // Initialize State Flags
    bPlayerCharged = false;
    bBossCharged = false;
    bPlayerDefending = false;
    bBossDefending = false;

    // --- REMOVED: InputManager creation ---
}

void ABossManager::BeginPlay()
{
    Super::BeginPlay();

    // 1. Find Actors
    BattlePlayerRef = Cast<ABattlePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    BossPawnRef = Cast<ABossPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossPawn::StaticClass()));

    // 2. Create Battle UI
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

    // --- REMOVED: InputManager Setup ---

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

    // Step 1: Start Sequence
    Sequence_PlayerAttack();
}

void ABossManager::Sequence_PlayerAttack()
{
    if (BattlePlayerRef)
    {
        if (PendingPlayerAction == EBattleAction::Attack) BattlePlayerRef->PlayAttackAnim();
        else if (PendingPlayerAction == EBattleAction::Defend) BattlePlayerRef->PlayDefendAnim();
    }

    FString ActionText = *UEnum::GetValueAsString(PendingPlayerAction);
    ActionText.RemoveFromStart("EBattleAction::");
    LogToUI(FString::Printf(TEXT("You chose to %s!"), *ActionText));

    // Wait before Boss reacts
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(BattleSequenceTimer, this, &ABossManager::Sequence_BossDecision, 2.0f, false);
    }
}

void ABossManager::Sequence_BossDecision()
{
    FGameState CurrentState;
    CurrentState.PlayerHP = PlayerHP;
    CurrentState.BossHP = BossHP;
    CurrentState.LastPlayerAction = PendingPlayerAction;
    // Pass persistent states into AI
    CurrentState.bPlayerDefending = bPlayerDefending;
    CurrentState.bBossDefending = bBossDefending;
    CurrentState.bPlayerCharged = bPlayerCharged;
    CurrentState.bBossCharged = bBossCharged;

    PendingBossAction = FMinimaxSolver::FindBestAction(CurrentState);
    LogToUI("The Boss is reacting...");

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(BattleSequenceTimer, this, &ABossManager::Sequence_BossAction, 0.5f, false);
    }
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

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(BattleSequenceTimer, this, &ABossManager::Sequence_ResolveTurn, 2.0f, false);
    }
}

void ABossManager::Sequence_ResolveTurn()
{
    // 1. CALCULATE DAMAGE (Using Local Function for Coin Logic)
    int32 DmgToBoss = CalculateDamageInternal(PendingPlayerAction, PendingBossAction, true);
    int32 DmgToPlayer = CalculateDamageInternal(PendingBossAction, PendingPlayerAction, false);

    // 2. APPLY DAMAGE
    BossHP = FMath::Max(0, BossHP - DmgToBoss);
    PlayerHP = FMath::Max(0, PlayerHP - DmgToPlayer);

    LogToUI(FString::Printf(TEXT("Result: You took %d dmg. Boss took %d dmg."), DmgToPlayer, DmgToBoss));

    // 3. UPDATE STATES FOR NEXT TURN
    ApplyActionLogic(PendingPlayerAction, false);
    ApplyActionLogic(PendingBossAction, true);

    if (PlayerHP <= 0 || BossHP <= 0)
    {
        EndBattle(BossHP <= 0);
    }
    else
    {
        bIsBusy = false;
        LogToUI("Your Turn...");
    }
}

void ABossManager::ApplyActionLogic(EBattleAction Action, bool bIsBossTurn)
{
    bool* ActorDefending = bIsBossTurn ? &bBossDefending : &bPlayerDefending;
    bool* ActorCharged = bIsBossTurn ? &bBossCharged : &bPlayerCharged;

    *ActorDefending = (Action == EBattleAction::Defend);

    if (Action == EBattleAction::Charge)
    {
        *ActorCharged = true;
        LogToUI(bIsBossTurn ? "Boss is charging up!" : "You are charging up!");
    }
    else if (Action == EBattleAction::Attack)
    {
        if (*ActorCharged)
        {
            *ActorCharged = false;
            LogToUI(bIsBossTurn ? "Boss unleashed Charge!" : "You unleashed Charge!");
        }
    }

    if (ActiveBattleWidget) ActiveBattleWidget->UpdateHP(PlayerHP, BossHP);
}

int32 ABossManager::CalculateDamageInternal(EBattleAction AttackerAction, EBattleAction DefenderAction, bool bIsPlayerAttacking)
{
    float Damage = 0.0f;
    float Multiplier = 1.0f;

    // --- COIN LOGIC: Base attack 20 + Total Coins ---
    float ActualBaseDamage = 20.0f;

    if (bIsPlayerAttacking)
    {
        UDSAGameInstance* GI = Cast<UDSAGameInstance>(GetGameInstance());
        if (GI)
        {
            ActualBaseDamage = 20.0f + (float)GI->TotalGlobalCoins;
            UE_LOG(LogTemp, Log, TEXT("Player Attack Power: %f (Coins: %d)"), ActualBaseDamage, GI->TotalGlobalCoins);
        }
    }

    if (AttackerAction == EBattleAction::Charge) Multiplier = 1.5f;
    else if (AttackerAction == EBattleAction::Attack) Multiplier = 1.0f;
    else return 0;

    Damage = ActualBaseDamage * Multiplier;

    // Check Guard logic (State from Previous Turn)
    bool bIsDefenderGuarding = (bIsPlayerAttacking ? bBossDefending : bPlayerDefending);
    bool bIsAttackerCharged = (bIsPlayerAttacking ? bPlayerCharged : bBossCharged);

    if (bIsDefenderGuarding)
    {
        // Shield Break Logic: Charge breaks shield, Normal attack gets blocked
        if (!bIsAttackerCharged) Damage = 0.0f;
    }

    if (!bIsPlayerAttacking) Damage *= 0.8f; // Boss deals less damage

    return FMath::Max(0, FMath::RoundToInt(Damage));
}

void ABossManager::EndBattle(bool bPlayerWon)
{
    if (bPlayerWon)
    {
        LogToUI("VICTORY! Boss Defeated.");
        if (BossPawnRef) BossPawnRef->PlayDeath();

        // Finalize Game in Save Data (Level 6 -> 7)
        UDSAGameInstance* GI = Cast<UDSAGameInstance>(GetGameInstance());
        if (GI)
        {
            GI->TotalGlobalCoins += 0;
            GI->CurrentLevelIndex++;
            GI->SaveState();
        }
    }
    else
    {
        LogToUI("DEFEAT! Try again.");
    }

    // --- SPAWN END LEVEL WIDGET ---
    if (ActiveBattleWidget) ActiveBattleWidget->SetVisibility(ESlateVisibility::Hidden);

    if (EndLevelWidgetClass)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
        {
            UUserWidget* EndWidget = CreateWidget<UUserWidget>(PC, EndLevelWidgetClass);
            if (EndWidget)
            {
                EndWidget->AddToViewport();
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
    // Note: No automatic transition. The player must click "Next Level" (Main Menu) on the widget.
}