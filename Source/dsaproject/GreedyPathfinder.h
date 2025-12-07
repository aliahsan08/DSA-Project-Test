#pragma once

#include "CoreMinimal.h"

// Simple Node for Greedy Search
struct FGridNode
{
    FVector Location;
    float HScore; // Heuristic only (Distance to goal)
    FGridNode* Parent;

    // Operator for sorting (Lower H score is better)
    bool operator<(const FGridNode& Other) const
    {
        return HScore < Other.HScore;
    }
};

class DSAPROJECT_API FGreedyPathfinder
{
public:
    // Greedy Best-First Search Algorithm
    // Returns a set of points to follow
    static TArray<FVector> FindPath(FVector Start, FVector End, UWorld* World);
};