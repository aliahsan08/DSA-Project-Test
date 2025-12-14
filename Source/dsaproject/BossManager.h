#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinimaxSolver.h"
#include "BattleWidget.h"
#include "BossManager.generated.h"

// Forward declarations
class ABattlePlayer;
class ABossPawn;

UCLASS()
class DSAPROJECT_API ABossManager : public AActor
{
    GENERATED_BODY()

public:
    ABossManager();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Stats")
        int32 PlayerHP;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle Stats")
        int32 BossHP;

    // --- REMOVED: UGameInputComponent* InputManager; ---

    // --- References to Actors ---
    UPROPERTY()
        ABattlePlayer* BattlePlayerRef;

    UPROPERTY()
        ABossPawn* BossPawnRef;

    // --- UI CONFIGURATION ---
    // The Battle Menu (Attack/Defend buttons)
    UPROPERTY(EditAnywhere, Category = "UI")
        TSubclassOf<UBattleWidget> BattleWidgetClass;

    // The Score Screen (Next Level button)
    UPROPERTY(EditAnywhere, Category = "UI")
        TSubclassOf<UUserWidget> EndLevelWidgetClass;

    UPROPERTY()
        UBattleWidget* ActiveBattleWidget;

    // Entry point for player input
    void ExecuteTurn(EBattleAction PlayerAction);

private:
    // --- State Variables ---
    bool bIsBusy;
    EBattleAction PendingPlayerAction;
    EBattleAction PendingBossAction;
    FTimerHandle BattleSequenceTimer;

    // Combat State Flags
    bool bPlayerCharged;
    bool bBossCharged;
    bool bPlayerDefending;
    bool bBossDefending;

    // --- Helper Functions ---
    void LogToUI(const FString& Message);

    // --- Sequence Steps ---
    void Sequence_PlayerAttack();
    void Sequence_BossDecision();
    void Sequence_BossAction();
    void Sequence_ResolveTurn();
    void EndBattle(bool bPlayerWon);

    void ApplyActionLogic(EBattleAction Action, bool bIsBossTurn);
    int32 CalculateDamageInternal(EBattleAction AttackerAction, EBattleAction DefenderAction, bool bIsPlayerAttacking);
};