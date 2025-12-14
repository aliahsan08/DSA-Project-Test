#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MySaveGame.h"
#include "DSAGameInstance.generated.h"

UCLASS()
class DSAPROJECT_API UDSAGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UDSAGameInstance();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
        FString CurrentProfileName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
        int32 CurrentLevelIndex;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
        int32 TotalGlobalCoins;

    // Returns FALSE if name is taken
    UFUNCTION(BlueprintCallable, Category = "SaveSystem")
        bool CreateNewGame(FString PlayerName);

    UFUNCTION(BlueprintCallable, Category = "SaveSystem")
        bool LoadGame(FString SlotName);

    UFUNCTION(BlueprintCallable, Category = "SaveSystem")
        void SaveState();

    // Updates data but DOES NOT save to disk immediately
    void CompleteLevel(int32 LevelScore, int32 LevelCoins);

    void OpenNextLevel();

    UFUNCTION(BlueprintCallable)
        TArray<FString> GetAllSaveSlots();
};