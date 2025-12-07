#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
// Removed Containers/Queue.h and Stack.h as requested
#include "DSAManagerComponent.generated.h"

// --- DSA: SCORE TREE NODE (for Player Progression/Ranking) ---
USTRUCT()
struct FScoreNode
{
    GENERATED_BODY()

        UPROPERTY()
        FString PlayerName;

    UPROPERTY()
        int32 Score;

    // Raw pointers for Tree structure
    FScoreNode* Left = nullptr;
    FScoreNode* Right = nullptr;

    // Destructor to recursively clean up
    ~FScoreNode()
    {
        if (Left) delete Left;
        if (Right) delete Right;
    }
};

// --- DSA: STACK ACTION (for Undo/Game State Reversibility) ---
USTRUCT()
struct FAction
{
    GENERATED_BODY()

        UPROPERTY()
        FString ActionDescription;

    UPROPERTY()
        FVector PlayerLocation = FVector::ZeroVector;
};

UCLASS(ClassGroup = (DSA), meta = (BlueprintSpawnableComponent))
class DSAPROJECT_API UDSAManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDSAManagerComponent();
    virtual void BeginDestroy() override;

    // --- 1. Tree Implementation (Player Progression) ---
    /** DSA: Root of the Binary Search Tree for scores. */
    FScoreNode* ScoreTreeRoot;

    void InsertScore(const FString& Name, int32 Score);

    // --- 2. Queue Implementation (Game State Event Log) ---
    /** DSA: TArray acting as a Queue (FIFO). Enqueue at End, Dequeue from 0. */
    UPROPERTY(VisibleAnywhere, Category = "DSA|Log")
        TArray<FString> EventLogQueue;

    void LogEvent(const FString& Event);
    void ProcessNextEvent();

    // --- 3. Stack Implementation (Game State Undo) ---
    /** DSA: TArray acting as a Stack (LIFO). Push to End, Pop from End. */
    UPROPERTY(VisibleAnywhere, Category = "DSA|History")
        TArray<FAction> ActionHistoryStack;

    void PushAction(const FAction& Action);
    FAction PopAction();

private:
    void InsertScoreRecursive(FScoreNode*& CurrentNode, const FString& Name, int32 Score);
};