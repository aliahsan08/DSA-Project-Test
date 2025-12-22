#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Queue.h" 
#include "MySaveGame.h" // For FLevelScore
#include "DSAManagerComponent.generated.h"

USTRUCT()
struct FAction
{
    GENERATED_BODY()
        UPROPERTY() FString ActionDescription;
    UPROPERTY() FVector PlayerLocation = FVector::ZeroVector;
};

UCLASS(ClassGroup = (DSA), meta = (BlueprintSpawnableComponent))
class DSAPROJECT_API UDSAManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDSAManagerComponent();

    // --- 1. Scoring (TArray + Binary Search) ---
    UPROPERTY(VisibleAnywhere, Category = "DSA|Score")
        TArray<FLevelScore> Scoreboard;

    // Now also tracks which level the score belongs to
    void AddScore(FString PlayerName, int32 Score, int32 LevelIndex);

    // --- 2. Queue (Events) ---
    TQueue<FString> EventLogQueue;
    void LogEvent(const FString& Event);

    // --- 3. Stack (Undo) ---
    TArray<FAction> ActionHistoryStack;
    void PushAction(const FAction& Action);
    FAction PopAction();
    void ClearHistory();
};