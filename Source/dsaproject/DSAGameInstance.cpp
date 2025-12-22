#include "DSAGameInstance.h"
#include "Kismet/GameplayStatics.h"

UDSAGameInstance::UDSAGameInstance()
{
    CurrentLevelIndex = 1;
    TotalGlobalCoins = 0;
}

bool UDSAGameInstance::CreateNewGame(FString PlayerName)
{
    // 1. UNIQUE NAME CHECK
    if (UGameplayStatics::DoesSaveGameExist(PlayerName, 0))
    {
        UE_LOG(LogTemp, Warning, TEXT("CreateNewGame Failed: Name '%s' already exists."), *PlayerName);
        return false; // Name taken
    }

    // 2. Initialize
    CurrentProfileName = PlayerName;
    CurrentLevelIndex = 1;
    TotalGlobalCoins = 0;

    // 3. Save initial state (Level 1)
    SaveState();

    // 4. Start
    OpenNextLevel();
    return true;
}

bool UDSAGameInstance::LoadGame(FString SlotName)
{
    if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        UMySaveGame* LoadData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
        if (LoadData)
        {
            CurrentProfileName = LoadData->PlayerName;
            CurrentLevelIndex = LoadData->CurrentLevelIndex;
            TotalGlobalCoins = LoadData->TotalCoinsCollected;
            HighScores = LoadData->HighScores;

            // Logic: Load to the NEXT level (stored in save file)
            OpenNextLevel();
            return true;
        }
    }
    return false;
}

void UDSAGameInstance::SaveState()
{
    if (CurrentProfileName.IsEmpty()) return;

    UMySaveGame* SaveInst = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    SaveInst->PlayerName = CurrentProfileName;

    // Logic: If completed Level 6, clamp index so player loads back into Boss Level (6)
    int32 SaveLevelIndex = (CurrentLevelIndex > 6) ? 6 : CurrentLevelIndex;

    SaveInst->CurrentLevelIndex = SaveLevelIndex;
    SaveInst->TotalCoinsCollected = TotalGlobalCoins;
    SaveInst->HighScores = HighScores;

    UGameplayStatics::SaveGameToSlot(SaveInst, CurrentProfileName, 0);
    UE_LOG(LogTemp, Warning, TEXT("Game Saved: %s at Level %d"), *CurrentProfileName, SaveLevelIndex);
}

void UDSAGameInstance::CompleteLevel(int32 LevelScore, int32 LevelCoins)
{
    // Update Memory ONLY
    TotalGlobalCoins += LevelCoins;
    CurrentLevelIndex++;
}

void UDSAGameInstance::OpenNextLevel()
{
    UGameplayStatics::SetGamePaused(GetWorld(), false);

    FString LevelName = "MainMenu";

    switch (CurrentLevelIndex)
    {
    case 1: LevelName = "Level1"; break;
    case 2: LevelName = "Level2"; break;
    case 3: LevelName = "Level3"; break;
    case 4: LevelName = "Level4"; break;
    case 5: LevelName = "Level5"; break;
    case 6: LevelName = "Level6"; break;
    default: LevelName = "MainMenu"; break;
    }

    UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}

TArray<FString> UDSAGameInstance::GetAllSaveSlots()
{
    TArray<FString> Slots;
    Slots.Add("Player1");
    return Slots;
}