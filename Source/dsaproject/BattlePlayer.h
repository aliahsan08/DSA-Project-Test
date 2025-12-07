#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "BattlePlayer.generated.h"

// Forward Declaration
class ABossManager;

UCLASS()
class DSAPROJECT_API ABattlePlayer : public APaperCharacter
{
    GENERATED_BODY()

public:
    ABattlePlayer();

protected:
    // --- Visuals ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* IdleAnimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* AttackAnimation;

    // NEW: Defend Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
        class UPaperFlipbook* DefendAnimation;

    // Camera setup
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        class UCameraComponent* SideViewCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
        class USpringArmComponent* CameraBoom;

    // --- State ---
    bool bIsAnimating; // Renamed from bIsAttacking to cover all anims
    FTimerHandle AnimationTimer;

    // --- Functions ---
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Input Handlers
    void InputAttack();
    void InputDefend();
    void InputCharge();

    // Animation Helper
    void ReturnToIdle();

public:
    // Called by Manager to trigger visuals
    void PlayAttackAnim();
    // NEW: Trigger Defend visual
    void PlayDefendAnim();
};