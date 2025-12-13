#include "DSAGameInstance.h"
#include "Kismet/GameplayStatics.h"

UDSAGameInstance::UDSAGameInstance()
{
    CurrentLevelIndex = 1;
    TotalGlobalCoins = 0;
}

void UDSAGameInstance::CreateNewGame(FString PlayerName)
{
    CurrentProfileName = PlayerName;
    CurrentLevelIndex = 1; // Start at Tutorial
    TotalGlobalCoins = 0;

    // Save immediately to create the file
    SaveState();
    OpenNextLevel();
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
    SaveInst->CurrentLevelIndex = CurrentLevelIndex;
    SaveInst->TotalCoinsCollected = TotalGlobalCoins;

    UGameplayStatics::SaveGameToSlot(SaveInst, CurrentProfileName, 0);
    UE_LOG(LogTemp, Warning, TEXT("Game Saved: %s at Level %d"), *CurrentProfileName, CurrentLevelIndex);
}

void UDSAGameInstance::CompleteLevel(int32 LevelScore, int32 LevelCoins)
{
    TotalGlobalCoins += LevelCoins;
    CurrentLevelIndex++;

    SaveState(); // Auto-save
    OpenNextLevel();
}

void UDSAGameInstance::OpenNextLevel()
{
    FString LevelName = "MainMenu";

    // 1=Tutorial, 2=Regular, 3=Survival, 4=Puzzle, 5=Stealth, 6=Boss
    switch (CurrentLevelIndex)
    {
    case 1: LevelName = "TutorialLevel"; break;
    case 2: LevelName = "RegularLevel"; break;
    case 3: LevelName = "SurvivalLevel"; break;
    case 4: LevelName = "PuzzleLevel"; break;
    case 5: LevelName = "StealthLevel"; break;
    case 6: LevelName = "BossLevel"; break;
    default: LevelName = "MainMenu"; break;
    }

    UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}

TArray<FString> UDSAGameInstance::GetAllSaveSlots()
{
    // Dummy implementation for UI testing
    TArray<FString> Slots;
    Slots.Add("Player1");
    return Slots;
}