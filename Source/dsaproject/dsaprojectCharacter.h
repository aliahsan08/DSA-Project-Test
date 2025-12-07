// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "dsaprojectCharacter.generated.h"

class UTextRenderComponent;
class UInventoryComponent;

UCLASS(config = Game)
class DSAPROJECT_API AdsaprojectCharacter : public APaperCharacter
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* SideViewCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DSA|Inventory", meta = (AllowPrivateAccess = "true"))
		UInventoryComponent* InventoryComponent;

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunningAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAnimation;

	void UpdateAnimation();
	void UpdateCharacter();
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

public:
	AdsaprojectCharacter();

	void DieAndRespawn();

	// --- MOVED TO PUBLIC FOR GameInputComponent ACCESS ---
	void MoveRight(float Value);
	// ----------------------------------------------------

	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
};