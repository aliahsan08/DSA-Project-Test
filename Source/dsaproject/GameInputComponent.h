#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameInputComponent.generated.h"

UENUM(BlueprintType)
enum class EInputMode : uint8
{
    EM_Platformer UMETA(DisplayName = "Platformer Movement"),
    EM_BattleMenu   UMETA(DisplayName = "Battle Menu Only")
};

UCLASS(ClassGroup = (Input), meta = (BlueprintSpawnableComponent))
class DSAPROJECT_API UGameInputComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGameInputComponent();

    UFUNCTION(BlueprintCallable, Category = "Input")
        void SetupInputMode(EInputMode NewMode);

protected:
    virtual void BeginPlay() override;

private:
    void BindPlatformerInputs(UInputComponent* InputComponent);
    void UnbindAllInputs(UInputComponent* InputComponent);

    // Handlers
    void HandleMoveRight(float Value);
    void HandleJump();
    void HandleStopJump();
    void HandleBattleAttack();
    void HandleBattleDefend();
    void HandleBattleCharge();
};