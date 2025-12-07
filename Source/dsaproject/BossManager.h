#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinimaxSolver.h"
#include "GameInputComponent.h"
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

    UPROPERTY(EditAnywhere, Category = "UI")
        TSubclassOf<UBattleWidget> BattleWidgetClass;

    UPROPERTY()
        UBattleWidget* ActiveBattleWidget;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
        UGameInputComponent* InputManager;

    // --- References to Actors ---
    UPROPERTY()
        ABattlePlayer* BattlePlayerRef;

    UPROPERTY()
        ABossPawn* BossPawnRef;

    void ExecuteTurn(EBattleAction PlayerAction);

private:
    bool bIsBusy;
    EBattleAction PendingPlayerAction;
    EBattleAction PendingBossAction;
    FTimerHandle BattleSequenceTimer; // Renamed to match CPP usage

    // State flags for next turn calculations
    bool bPlayerCharged;
    bool bBossCharged;
    bool bPlayerDefending;
    bool bBossDefending;

    void LogToUI(const FString& Message);

    // -- SEQUENCE STEPS (Unified Names) --
    void Sequence_PlayerAction();
    void Sequence_BossDecision();
    void Sequence_BossAction();
    void Sequence_ResolveTurn();
    void EndBattle(bool bPlayerWon);

    void ApplyActionLogic(EBattleAction Action, bool bIsBossTurn);
};