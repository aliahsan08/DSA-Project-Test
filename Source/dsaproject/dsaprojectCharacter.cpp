// Copyright Epic Games, Inc. All Rights Reserved.

#include "dsaprojectCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "InventoryComponent.h" 
#include "dsaprojectGameMode.h" 
#include "DSAManagerComponent.h"
#include "DSAGameInstance.h" 
#include "Kismet/GameplayStatics.h" 

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

AdsaprojectCharacter::AdsaprojectCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	GetSprite()->SetIsReplicated(true);
	bReplicates = true;
}

void AdsaprojectCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void AdsaprojectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateCharacter();
}

void AdsaprojectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// 1. Standard Platformer Controls
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AdsaprojectCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AdsaprojectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AdsaprojectCharacter::TouchStopped);

	// 2. Global System Controls
	// Save Game (Removed as requested - now handled by UI/EndLevel)
	// Escape to Main Menu (Added as requested)
	PlayerInputComponent->BindKey(EKeys::Escape, IE_Pressed, this, &AdsaprojectCharacter::ReturnToMainMenu);
}

void AdsaprojectCharacter::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}

void AdsaprojectCharacter::MoveRight(float Value)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AdsaprojectCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	Jump();
}

void AdsaprojectCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void AdsaprojectCharacter::UpdateCharacter()
{
	UpdateAnimation();

	// Undo Stack Logic (DSA)
	AdsaprojectGameMode* GameMode = Cast<AdsaprojectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode && GameMode->DSAManager)
	{
		if (FMath::Fmod(GetWorld()->GetTimeSeconds(), 0.5f) < GetWorld()->GetDeltaSeconds())
		{
			FAction NewAction;
			NewAction.ActionDescription = "Movement Checkpoint";
			NewAction.PlayerLocation = GetActorLocation();
			GameMode->DSAManager->PushAction(NewAction);
		}
	}

	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

void AdsaprojectCharacter::DieAndRespawn()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC) DisableInput(PC);

	SetActorHiddenInGame(true);

	AdsaprojectGameMode* GameMode = Cast<AdsaprojectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player died, calling GameMode::RespawnPlayer."));
		GameMode->RespawnPlayer(this);
	}
}