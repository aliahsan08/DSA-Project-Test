#pragma once

#include "CoreMinimal.h"

// Define a simple node for the pathfinding grid
struct FNode
{
    FVector Location;
    float GScore; // Cost from start
    float HScore; // Heuristic
    float FScore; // G + H
    FNode* Parent;
};

class DSAPROJECT_API FSAStarPathfinder
{
public:
    // DSA: A* Search Algorithm. Finds path from Start to End.
    static TArray<FVector> FindPath(FVector Start, FVector End, UWorld* World);

private:
    static float CalculateHScore(const FVector& A, const FVector& B)
    {
        return FMath::Abs(A.X - B.X) + FMath::Abs(A.Z - B.Z);
    }
};