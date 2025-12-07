#include "GreedyPathfinder.h"
#include "DrawDebugHelpers.h"

TArray<FVector> FGreedyPathfinder::FindPath(FVector Start, FVector End, UWorld* World)
{
    // OUTPUT PATH
    TArray<FVector> Path;

    // 1. GREEDY SIMPLIFICATION
    // In a pure 2D side scroller without complex mazes, a full node-graph search 
    // is often overkill. We can implement a "Continuous Greedy" step.
    // This simulates the algorithm by stepping towards the goal 
    // and checking for simple blockage.

    FVector CurrentPos = Start;
    FVector Direction = (End - Start).GetSafeNormal();

    // We break the path into small steps (Waypoints)
    // This represents the "Nodes" the algorithm selected as 'Best' based on H-Score.
    int32 MaxSteps = 10;
    float StepSize = FVector::Dist(Start, End) / MaxSteps;

    // If target is very close, just go there
    if (StepSize < 10.0f)
    {
        Path.Add(End);
        return Path;
    }

    for (int32 i = 0; i < MaxSteps; i++)
    {
        // Calculate next theoretical node based on Heuristic (closest to goal)
        FVector NextPoint = CurrentPos + (Direction * StepSize);

        // Check if this node is valid (Not inside a wall)
        FHitResult Hit;
        FCollisionQueryParams Params;
        bool bHit = World->LineTraceSingleByChannel(Hit, CurrentPos, NextPoint, ECC_Visibility, Params);

        if (!bHit)
        {
            Path.Add(NextPoint);
            CurrentPos = NextPoint;
        }
        else
        {
            // Wall detected! Greedy fails here (it's not smart enough to backtrack efficiently)
            // But for this platformer, stopping is the correct behavior.
            break;
        }
    }

    Path.Add(End);
    return Path;
}