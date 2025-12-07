#include "EnemyMonster.h"
#include "dsaprojectCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" 
#include "dsaprojectGameMode.h" 

AEnemyMonster::AEnemyMonster()
{
    PrimaryActorTick.bCanEverTick = true;
    MoveSpeed = 150.0f;
    LedgeCheckDistance = 70.0f;
    PatrolDirection = 1.0f;

    // CRITICAL FIX: Set response to Overlap in C++ to ensure the lethal trigger fires continuously.
    // We rely on the Player's Capsule to provide the physical 'block' feeling.
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AEnemyMonster::BeginPlay()
{
    Super::BeginPlay();

    UCapsuleComponent* Capsule = GetCapsuleComponent();
    if (Capsule)
    {
        // Bind Overlap event (This is the lethal trigger)
        Capsule->OnComponentBeginOverlap.AddDynamic(this, &AEnemyMonster::OnOverlapBegin);
    }

    // DSA REGISTRATION: Register this enemy to the GameMode's TSet
    AdsaprojectGameMode* GameMode = Cast<AdsaprojectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->ActiveEnemies.Add(this);
        UE_LOG(LogTemp, Log, TEXT("DSA: Enemy Registered in TSet: %s"), *GetName());
    }
}

void AEnemyMonster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // BASE BEHAVIOR: Always Patrol.
    Patrol(DeltaTime);
}

// Helper function to check for floor ahead (used by Patrol and Chase)
bool AEnemyMonster::CheckForLedge(FVector DirectionVector)
{
    FVector Start = GetActorLocation();

    // Move the start point forward by LedgeCheckDistance
    Start.X += (DirectionVector.X * LedgeCheckDistance);

    // The end point is directly below the start point
    FVector End = Start - FVector(0.0f, 0.0f, 150.0f);

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    // Perform a Line Trace downwards
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

    // Draw the debug line to see the trace in the editor
    DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 0.1f, 0, 1.0f);

    return bHit;
}

// Logic for walking back and forth (Patrol state)
void AEnemyMonster::Patrol(float DeltaTime)
{
    FVector CurrentLocation = GetActorLocation();
    FVector MoveDir = FVector(PatrolDirection, 0.0f, 0.0f);

    // If the ledge check fails (no floor ahead), reverse direction
    if (!CheckForLedge(MoveDir))
    {
        PatrolDirection *= -1.0f;
        return;
    }

    FVector NewLocation = CurrentLocation + (MoveDir * MoveSpeed * DeltaTime);
    SetActorLocation(NewLocation);

    // Update rotation for visual feedback
    if (PatrolDirection < 0.0f) SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
    else SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// --- COLLISION HANDLERS ---

// 1. Handle Overlaps (The lethal, continuous trigger)
void AEnemyMonster::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AdsaprojectCharacter* Player = Cast<AdsaprojectCharacter>(OtherActor);
    if (Player)
    {
        // THIS IS THE INSTANT DEATH LOGIC
        UE_LOG(LogTemp, Warning, TEXT("Enemy OVERLAPPED Player! Killing now."));
        Player->DieAndRespawn();
    }
}

// 2. NotifyHit (Only keeps the required function definition, no custom death logic here)
void AEnemyMonster::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    // NotifyHit is still required by the ACharacter hierarchy.
    // Since we are using Overlap for lethality, we just call the parent function 
    // and rely on OnOverlapBegin to handle the player death event.
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
}