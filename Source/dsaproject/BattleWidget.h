#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BattleWidget.generated.h"

// Forward declaration
class ABossManager;

/**
 * UBattleWidget
 * C++ Logic for the Battle Menu.
 * Replaces Blueprint Graph logic entirely.
 */
UCLASS()
class DSAPROJECT_API UBattleWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // --- 1. BIND WIDGETS ---
    // The meta=(BindWidget) tag tells Unreal: "Find a button in the Designer 
    // named EXACTLY 'Btn_Attack' and link it to this C++ variable."

    UPROPERTY(meta = (BindWidget))
        UButton* Btn_Attack;

    UPROPERTY(meta = (BindWidget))
        UButton* Btn_Defend;

    UPROPERTY(meta = (BindWidget))
        UButton* Btn_Charge;

    UPROPERTY(meta = (BindWidget))
        UTextBlock* Txt_BattleLog;

    UPROPERTY(meta = (BindWidget))
        UTextBlock* Txt_PlayerHP;

    UPROPERTY(meta = (BindWidget))
        UTextBlock* Txt_BossHP;

    // --- 2. SETUP ---
    virtual void NativeConstruct() override;

    // --- 3. PUBLIC FUNCTIONS called by BossManager ---
    void UpdateLog(FString Message);
    void UpdateHP(int32 PlayerHP, int32 BossHP);

private:
    // Reference to the Manager to send commands
    ABossManager* BossManagerRef;

    // Click Handlers
    UFUNCTION()
        void OnAttackClicked();

    UFUNCTION()
        void OnDefendClicked();

    UFUNCTION()
        void OnChargeClicked();
};