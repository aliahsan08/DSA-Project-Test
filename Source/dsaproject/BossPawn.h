#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "BossPawn.generated.h"

UCLASS()
class DSAPROJECT_API ABossPawn : public AActor
{
    GENERATED_BODY()

public:
    ABossPawn();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals")
        UPaperFlipbookComponent* Sprite;

    // Assign these in Blueprint
    UPROPERTY(EditAnywhere, Category = "Animations")
        UPaperFlipbook* IdleAnim;

    UPROPERTY(EditAnywhere, Category = "Animations")
        UPaperFlipbook* AttackAnim;

    // NEW: Defend Animation
    UPROPERTY(EditAnywhere, Category = "Animations")
        UPaperFlipbook* DefendAnim;

    // NEW: Death Animation
    UPROPERTY(EditAnywhere, Category = "Animations")
        UPaperFlipbook* DeathAnim;

    // Functions to trigger visuals
    void PlayAttack();
    void PlayDefend();
    void PlayDeath();

protected:
    virtual void BeginPlay() override;

private:
    FTimerHandle AnimTimer;
    void ReturnToIdle();

    // NEW: Hides sprite after death anim finishes
    void FinishDeath();
};