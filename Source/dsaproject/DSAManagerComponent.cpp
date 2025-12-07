#include "DSAManagerComponent.h"

UDSAManagerComponent::UDSAManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    ScoreTreeRoot = nullptr;
}

void UDSAManagerComponent::BeginDestroy()
{
    if (ScoreTreeRoot)
    {
        delete ScoreTreeRoot;
        ScoreTreeRoot = nullptr;
    }
    Super::BeginDestroy();
}

// =========================================================================
// 1. Score Tree Implementation (Insertion)
// =========================================================================
void UDSAManagerComponent::InsertScore(const FString& Name, int32 Score)
{
    UE_LOG(LogTemp, Log, TEXT("DSA: Inserting score %d for %s into BST."), Score, *Name);
    InsertScoreRecursive(ScoreTreeRoot, Name, Score);
}

void UDSAManagerComponent::InsertScoreRecursive(FScoreNode*& CurrentNode, const FString& Name, int32 Score)
{
    if (CurrentNode == nullptr)
    {
        CurrentNode = new FScoreNode();
        CurrentNode->PlayerName = Name;
        CurrentNode->Score = Score;
        CurrentNode->Left = nullptr;
        CurrentNode->Right = nullptr;
    }
    else if (Score > CurrentNode->Score)
    {
        InsertScoreRecursive(CurrentNode->Right, Name, Score);
    }
    else
    {
        InsertScoreRecursive(CurrentNode->Left, Name, Score);
    }
}

// =========================================================================
// 2. Queue Implementation (TArray as FIFO)
// =========================================================================
void UDSAManagerComponent::LogEvent(const FString& Event)
{
    // Enqueue: Add to the end of the array
    EventLogQueue.Add(Event);
    UE_LOG(LogTemp, Log, TEXT("DSA: Event Enqueued: %s"), *Event);
}

void UDSAManagerComponent::ProcessNextEvent()
{
    // Dequeue: Remove from the start (Index 0)
    if (EventLogQueue.Num() > 0)
    {
        FString NextEvent = EventLogQueue[0];
        EventLogQueue.RemoveAt(0); // Shifts remaining elements down
        UE_LOG(LogTemp, Warning, TEXT("DSA: Event Dequeued and Processed: %s"), *NextEvent);
    }
}

// =========================================================================
// 3. Stack Implementation (TArray as LIFO)
// =========================================================================
void UDSAManagerComponent::PushAction(const FAction& Action)
{
    // Push: Add to the end
    ActionHistoryStack.Push(Action);
    UE_LOG(LogTemp, Log, TEXT("DSA: Action Pushed (LIFO): %s"), *Action.ActionDescription);
}

FAction UDSAManagerComponent::PopAction()
{
    // Pop: Remove from the end
    if (ActionHistoryStack.Num() > 0)
    {
        // TArray::Pop returns the element in UE
        FAction LastAction = ActionHistoryStack.Pop();
        UE_LOG(LogTemp, Warning, TEXT("DSA: Action Popped (LIFO): %s"), *LastAction.ActionDescription);
        return LastAction;
    }

    return FAction();
}