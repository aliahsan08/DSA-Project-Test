#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FLevelScore
{
    GENERATED_BODY()

        UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
        FString PlayerName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
        int32 Score;

    // Operator for Sorting (Descending Order)
    bool operator<(const FLevelScore& Other) const
    {
        return Score > Other.Score;
    }
};

UCLASS()
class DSAPROJECT_API UMySaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UMySaveGame();

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString PlayerName;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        int32 CurrentLevelIndex; // 1 to 6

    UPROPERTY(VisibleAnywhere, Category = Basic)
        int32 TotalCoinsCollected; // Persists across game

        // Stores top scores for the current session/profile
    UPROPERTY(VisibleAnywhere, Category = Basic)
        TArray<FLevelScore> HighScores;
};