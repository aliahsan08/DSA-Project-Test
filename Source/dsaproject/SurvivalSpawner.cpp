#include "SurvivalSpawner.h"
#include "dsaprojectGameMode.h"
#include "dsaprojectCharacter.h" 
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h" 

ASurvivalSpawner::ASurvivalSpawner()
{
    PrimaryActorTick.bCanEverTick = true;
    TimeElapsed = 0.0f;
    bLevelActive = true;
}

void ASurvivalSpawner::BeginPlay()
{
    Super::BeginPlay();

    if (!EnemyClass)
    {
        UE_LOG(LogTemp, Error, TEXT("DSA ERROR: 'Enemy Class' is NONE! Please assign BP_GreedyEnemy in the Details Panel of BP_SurvivalManager."));
        return;
    }

    // --- ALGORITHM: OBJECT POOL INITIALIZATION ---
    // FIX: Spawn at Z=0 (Safe) instead of -5000 (Kill Zone)
    // Since we call SetActorHiddenInGame(true) immediately, they won't be seen.
    FVector HiddenLoc(0.0f, 0.0f, 500.0f);

    for (int32 i = 0; i < PoolSize; i++)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AEnemyMonster* NewEnemy = GetWorld()->SpawnActor<AEnemyMonster>(EnemyClass, HiddenLoc, FRotator::ZeroRotator, SpawnParams);

        if (NewEnemy)
        {
            // Initial Deactivation
            NewEnemy->SetActorHiddenInGame(true);
            NewEnemy->SetActorTickEnabled(false);
            NewEnemy->SetActorEnableCollision(false);
            EnemyPool.Enqueue(NewEnemy);
        }
    }

    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASurvivalSpawner::ActivateNextEnemy, SpawnInterval, true);

    UE_LOG(LogTemp, Warning, TEXT("DSA: Survival Level Started. Survive for %f seconds!"), TimeLimit);
}

// ... (Rest of the file remains exactly the same as previous: Tick, ActivateNextEnemy, WinLevel) ...
void ASurvivalSpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bLevelActive) return;

    TimeElapsed += DeltaTime;

    if (TimeElapsed >= TimeLimit)
    {
        WinLevel();
    }

    if (GEngine)
    {
        FString TimeMsg = FString::Printf(TEXT("Survive: %.1f / %.1f"), TimeElapsed, TimeLimit);
        GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Red, TimeMsg);
    }
}

void ASurvivalSpawner::ActivateNextEnemy()
{
    if (EnemyPool.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("DSA WARNING: Enemy Pool is empty! (Wait for recycle)"));
        return;
    }

    if (SpawnPoints.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("DSA ERROR: 'Spawn Points' array is EMPTY!"));
        return;
    }

    AEnemyMonster* EnemyToSpawn;
    if (EnemyPool.Dequeue(EnemyToSpawn))
    {
        // FIX: Check IsValid to ensure the actor wasn't destroyed by engine cleanup
        if (IsValid(EnemyToSpawn))
        {
            int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
            AActor* Spot = SpawnPoints[RandomIndex];

            if (Spot)
            {
                // 3. Reset State ("Recycle")
                FVector SpawnLoc = Spot->GetActorLocation();
                SpawnLoc.Y = 0.0f; // Force to Player Plane

                FRotator SpawnRot = FRotator(0.0f, -90.0f, 0.0f);

                EnemyToSpawn->SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
                EnemyToSpawn->SetActorLocationAndRotation(SpawnLoc, SpawnRot);

                EnemyToSpawn->SetActorHiddenInGame(false);
                if (EnemyToSpawn->GetRootComponent())
                {
                    EnemyToSpawn->GetRootComponent()->SetVisibility(true, true);
                }

                EnemyToSpawn->SetActorTickEnabled(true);
                EnemyToSpawn->SetActorEnableCollision(true);

                DrawDebugBox(GetWorld(), SpawnLoc, FVector(50, 50, 50), FColor::Green, false, 3.0f);

                UE_LOG(LogTemp, Log, TEXT("DSA: Spawning Enemy from Pool at %s (Loc: %s)"), *Spot->GetName(), *SpawnLoc.ToString());
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("DSA ERROR: Enemy in pool was invalid (Destroyed via KillZ?)"));
        }

        EnemyPool.Enqueue(EnemyToSpawn);
    }
}

void ASurvivalSpawner::WinLevel()
{
    bLevelActive = false;
    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

    AdsaprojectGameMode* GameMode = Cast<AdsaprojectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        AdsaprojectCharacter* Player = Cast<AdsaprojectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        GameMode->FinishLevel(Player);
    }
}