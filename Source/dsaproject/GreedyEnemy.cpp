#include "GreedyEnemy.h"
#include "dsaprojectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GreedyPathfinder.h" // Algorithm Include

AGreedyEnemy::AGreedyEnemy()
{
    // Defaults for the "Smart" enemy
    SightRange = 600.0f;
    SightHeight = 150.0f;
    CurrentPathIndex = 0;
}

void AGreedyEnemy::Tick(float DeltaTime)
{
    // Do NOT call Super::Tick(DeltaTime) because we are replacing the logic completely.
    // The Base class Tick only calls Patrol(). We want to choose.

    AdsaprojectCharacter* Player = Cast<AdsaprojectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    // --- AI DECISION TREE ---
    bool bShouldChase = false;

    if (Player)
    {
        // 1. ELLIPTICAL CULLING (Optimization Algorithm)
        FVector MyLoc = GetActorLocation();
        FVector PlayerLoc = Player->GetActorLocation();

        float DX = FMath::Abs(MyLoc.X - PlayerLoc.X);
        float DZ = FMath::Abs(MyLoc.Z - PlayerLoc.Z);

        float RX = (SightRange > 0.0f) ? SightRange : 1.0f;
        float RZ = (SightHeight > 0.0f) ? SightHeight : 1.0f;

        float EllipseValue = FMath::Square(DX / RX) + FMath::Square(DZ / RZ);

        if (EllipseValue <= 1.0f)
        {
            bShouldChase = true;
        }
    }

    // 2. STATE EXECUTION
    if (bShouldChase)
    {
        ChasePlayer(DeltaTime, Player);
    }
    else
    {
        // Fallback to Base Class behavior
        Patrol(DeltaTime);
    }
}

void AGreedyEnemy::ChasePlayer(float DeltaTime, AActor* Player)
{
    // ALGORITHM: Greedy Best-First Search
    if (PathToPlayer.Num() == 0 || CurrentPathIndex >= PathToPlayer.Num())
    {
        TargetLocation = Player->GetActorLocation();
        PathToPlayer = FGreedyPathfinder::FindPath(GetActorLocation(), TargetLocation, GetWorld());
        CurrentPathIndex = 0;
    }

    if (PathToPlayer.Num() > 0 && CurrentPathIndex < PathToPlayer.Num())
    {
        FVector NextPoint = PathToPlayer[CurrentPathIndex];
        FVector CurrentLocation = GetActorLocation();

        FVector Direction = (NextPoint - CurrentLocation);
        Direction.Z = 0.0f;
        Direction.Y = 0.0f;
        Direction.Normalize();

        // Use Base Class helper to check safety
        if (!CheckForLedge(Direction))
        {
            PathToPlayer.Empty();
            return;
        }

        FVector NewLocation = CurrentLocation + Direction * (MoveSpeed * 1.5f) * DeltaTime;
        SetActorLocation(NewLocation);

        if (FVector::DistSquared2D(NewLocation, NextPoint) < FMath::Square(20.0f))
        {
            CurrentPathIndex++;
        }

        if (Direction.X < 0.0f) SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
        else if (Direction.X > 0.0f) SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
    }
}