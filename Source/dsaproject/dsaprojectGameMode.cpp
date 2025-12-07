// Copyright Epic Games, Inc. All Rights Reserved.

#include "dsaprojectGameMode.h"
#include "dsaprojectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "DSAManagerComponent.h" 

AdsaprojectGameMode::AdsaprojectGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AdsaprojectCharacter::StaticClass();
	CurrentScore = 0;

	// Initialize the DSA Manager Component
	DSAManager = CreateDefaultSubobject<UDSAManagerComponent>(TEXT("DSAManagerComponent"));
}

void AdsaprojectGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Find a PlayerStart for the initial spawn location
	AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
	if (PlayerStart)
	{
		SpawnLocation = PlayerStart->GetActorLocation();
	}

	UE_LOG(LogTemp, Log, TEXT("GameMode Initialized. Score: %d"), CurrentScore);
}

void AdsaprojectGameMode::RespawnPlayer(AdsaprojectCharacter* DeadPlayer)
{
	if (DeadPlayer)
	{
		// Teleport player back to the spawn location
		DeadPlayer->SetActorLocation(SpawnLocation);
		DeadPlayer->SetActorRotation(FRotator::ZeroRotator);

		// Re-enable player controls
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			DeadPlayer->EnableInput(PC);
			DeadPlayer->SetActorHiddenInGame(false);
		}

		UE_LOG(LogTemp, Warning, TEXT("Player Respawned."));
	}
}

void AdsaprojectGameMode::FinishLevel(AdsaprojectCharacter* Player)
{
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("LEVEL COMPLETE! Final Score: %d"), CurrentScore);

		// DSA: Use the Score Tree (BST) for Player Progression
		if (DSAManager)
		{
			// Generate a dummy name or use PlayerState name
			FString PlayerName = FString::Printf(TEXT("Player_%d"), FMath::RandRange(100, 999));
			DSAManager->InsertScore(PlayerName, CurrentScore);
		}

		// Logic to load next level would go here
	}
}

void AdsaprojectGameMode::AddScore(int32 ScoreToAdd)
{
	CurrentScore += ScoreToAdd;
	UE_LOG(LogTemp, Log, TEXT("Score Updated: %d (+%d)"), CurrentScore, ScoreToAdd);
}