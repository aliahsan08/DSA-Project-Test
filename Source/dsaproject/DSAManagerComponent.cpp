#include "DSAManagerComponent.h"
#include "Algo/BinarySearch.h"

UDSAManagerComponent::UDSAManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// --- ALGORITHM: BINARY SEARCH INSERTION ---
// "Replace BST with TArray and use binary search"
void UDSAManagerComponent::AddScore(FString Name, int32 Score, int32 LevelIndex)
{
    FLevelScore NewEntry;
    NewEntry.PlayerName = Name;
    NewEntry.Score = Score;
    NewEntry.LevelIndex = LevelIndex;

    // Binary Search to find insertion index (Sorted Descending)
    // We use a Predicate to tell LowerBound how to sort
    int32 InsertIndex = Algo::LowerBound(Scoreboard, NewEntry, [](const FLevelScore& A, const FLevelScore& B) {
        return A.Score > B.Score;
        });

    Scoreboard.Insert(NewEntry, InsertIndex);

    UE_LOG(LogTemp, Warning, TEXT("DSA: Score %d inserted at index %d using Binary Search."), Score, InsertIndex);
}

void UDSAManagerComponent::LogEvent(const FString& Event)
{
    EventLogQueue.Enqueue(Event);
}

void UDSAManagerComponent::PushAction(const FAction& Action)
{
    ActionHistoryStack.Push(Action);
}

FAction UDSAManagerComponent::PopAction()
{
    if (ActionHistoryStack.Num() > 0) return ActionHistoryStack.Pop();
    return FAction();
}

void UDSAManagerComponent::ClearHistory()
{
    ActionHistoryStack.Empty();
}