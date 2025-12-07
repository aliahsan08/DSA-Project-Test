#include "StealthEnemy.h"
#include "dsaprojectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" 
#include "Kismet/KismetMathLibrary.h"

AStealthEnemy::AStealthEnemy()
{
    // Defaults
    SightRange = 500.0f;
    VisionAngle = 90.0f;
    bShowDebugVision = true; // Helpful for level design
}

void AStealthEnemy::Tick(float DeltaTime)
{
    // 1. GET PLAYER
    AdsaprojectCharacter* Player = Cast<AdsaprojectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (Player)
    {
        // 2. RUN STEALTH ALGORITHM
        if (CanSeePlayer(Player))
        {
            // CAUGHT!
            GameOver();
            return; // Stop moving
        }
    }

    // 3. FALLBACK TO PATROL
    // Since we are not chasing, we just walk back and forth using the Base Class logic
    Patrol(DeltaTime);
}

bool AStealthEnemy::CanSeePlayer(AActor* Player)
{
    if (!Player) return false;

    FVector MyLoc = GetActorLocation();
    FVector PlayerLoc = Player->GetActorLocation();
    FVector ForwardDir = GetActorForwardVector(); // The direction sprite/capsule is facing

    // --- STEP 1: DISTANCE CHECK (Optimization) ---
    // If player is too far, don't do expensive math
    float DistSq = FVector::DistSquared(MyLoc, PlayerLoc);
    if (DistSq > (SightRange * SightRange))
    {
        return false;
    }

    // --- STEP 2: ANGLE CHECK (Dot Product Algorithm) ---
    // Calculate direction to player
    FVector DirToPlayer = (PlayerLoc - MyLoc).GetSafeNormal();

    // In Paper2D, "Forward" might be X or -X depending on rotation.
    // If your character rotates strictly 0/180, ForwardVector works.
    // If you flip scale, you might need to manually construct forward vector based on PatrolDirection.

    // Dot Product: 1.0 = Directly Ahead, 0.0 = 90 degrees side, -1.0 = Behind
    float DotResult = FVector::DotProduct(ForwardDir, DirToPlayer);

    // Convert Degrees to Cosine Threshold (e.g., 90deg -> 45deg half-angle -> Cos(45) ~= 0.707)
    float AngleThreshold = FMath::Cos(FMath::DegreesToRadians(VisionAngle / 2.0f));

    // If DotResult is SMALLER than threshold, player is OUTSIDE cone.
    if (DotResult < AngleThreshold)
    {
        return false;
    }

    // --- STEP 3: OCCLUSION CHECK (Raycast) ---
    // We see them mathematically, but is there a wall?
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // Ignore self

    // Trace from eyes to player
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, MyLoc, PlayerLoc, ECC_Visibility, Params);

    if (bHit)
    {
        // Debug Line
        if (bShowDebugVision)
        {
            DrawDebugLine(GetWorld(), MyLoc, Hit.Location, FColor::Red, false, -1.0f, 0, 2.0f);
        }

        // Did we hit the player?
        if (Hit.GetActor() == Player)
        {
            return true; // SEEN!
        }
    }

    return false; // Blocked by wall
}

void AStealthEnemy::GameOver()
{
    UE_LOG(LogTemp, Error, TEXT("STEALTH FAILED: Player Spotted!"));

    // Restart Level Logic
    // In a real game, you might show a "CAUGHT" widget first.
    // Here, we instantly restart to punish the player.
    FString CurrentLevelName = GetWorld()->GetMapName();
    CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
    UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));
}