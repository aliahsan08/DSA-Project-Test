#include "dsaprojectGameMode.h"
#include "dsaprojectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "DSAManagerComponent.h" 
#include "DSAGameInstance.h"
#include "CircuitManager.h" // Needed for Puzzle Condition

AdsaprojectGameMode::AdsaprojectGameMode()
{
    DefaultPawnClass = AdsaprojectCharacter::StaticClass();
    PrimaryActorTick.bCanEverTick = true; // Enable Tick for Timer

    DSAManager = CreateDefaultSubobject<UDSAManagerComponent>(TEXT("DSAManagerComponent"));

    CurrentScore = 700;
    CoinsCollectedInLevel = 0;
    TimeElapsed = 0.0f;
}

void AdsaprojectGameMode::BeginPlay()
{
    Super::BeginPlay();
    AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
    if (PlayerStart) SpawnLocation = PlayerStart->GetActorLocation();
}

void AdsaprojectGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeElapsed += DeltaTime;

    // Score Logic: Drops by 10 every second
    int32 TimePenalty = FMath::FloorToInt(TimeElapsed) * 10;

    // Logic: At 60 seconds (1 min), drop by 600 (handled by formula: 60 * 10 = 600)
    // Constraint: Minimum score before coins is 100
    int32 RawScore = BaseScore - TimePenalty;

    // If timer hits a minute (60s), RawScore is 100. If it goes below, clamp to 100.
    if (RawScore < 100)
    {
        RawScore = 100;
    }

    // Add Coins (100 per coin)
    CurrentScore = RawScore + (CoinsCollectedInLevel * 100);
}

void AdsaprojectGameMode::AddScore(int32 ScoreToAdd)
{
    // In this specific rule set, "ScoreToAdd" is the coin value (usually 1)
    // We track total coins separately to calculate the bonus
    CoinsCollectedInLevel += ScoreToAdd;
}

void AdsaprojectGameMode::RespawnPlayer(AdsaprojectCharacter* DeadPlayer)
{
    if (DeadPlayer)
    {
        // REQUIREMENT: "Actions get popped" -> Clear History
        if (DSAManager) DSAManager->ClearHistory();

        // REQUIREMENT: "Everything respawns" -> Restart Level
        FString CurrentLevelName = GetWorld()->GetMapName();
        CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
        UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));
    }
}

void AdsaprojectGameMode::FinishLevel(AdsaprojectCharacter* Player)
{
    // --- PUZZLE LEVEL CONDITION CHECK (Wires Fixed) ---
    // We check if a CircuitManager exists. If so, we are in the puzzle level.
    ACircuitManager* CircuitMgr = Cast<ACircuitManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACircuitManager::StaticClass()));
    if (CircuitMgr)
    {
        // If Circuit Manager exists, we MUST solve it to pass
        if (!CircuitMgr->IsCircuitComplete())
        {
            UE_LOG(LogTemp, Warning, TEXT("Level Finished Failed: Circuit not complete!"));
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Locked! Fix the Wires first."));
            return; // Door is locked
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("LEVEL COMPLETE! Final Score: %d"), CurrentScore);

    // Save Data to GameInstance and Move On
    UDSAGameInstance* GI = Cast<UDSAGameInstance>(GetGameInstance());
    if (GI)
    {
        // Add score to leaderboard
        if (DSAManager) DSAManager->AddScore(GI->CurrentProfileName, CurrentScore);

        // Pass data and transition
        GI->CompleteLevel(CurrentScore, CoinsCollectedInLevel);
    }
}