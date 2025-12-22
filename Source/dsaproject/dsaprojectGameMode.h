#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MySaveGame.h" 
#include "Blueprint/UserWidget.h" // Required for CreateWidget
#include "dsaprojectGameMode.generated.h"

class AdsaprojectCharacter;
class AEnemyMonster;
class UDSAManagerComponent;

UCLASS()
class DSAPROJECT_API AdsaprojectGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    AdsaprojectGameMode();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
        FVector SpawnLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DSA")
        UDSAManagerComponent* DSAManager;

    UPROPERTY(VisibleAnywhere, Category = "Enemies")
        TSet<AEnemyMonster*> ActiveEnemies;

    // --- UI CONFIG ---
    // Assign WBP_EndLevel here in the Editor
    UPROPERTY(EditAnywhere, Category = "UI")
        TSubclassOf<UUserWidget> EndLevelWidgetClass;

    // --- SCORING SYSTEM ---
    int32 BaseScore = 700;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
        int32 CurrentScore;

    // NEW: Which level was just completed (used for per-level highscores)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
        int32 CompletedLevelIndex;

    // Prevents multiple finish calls and flags that gameplay should be frozen
    bool bLevelFinished;

    int32 CoinsCollectedInLevel;
    float TimeElapsed;

    void RespawnPlayer(AdsaprojectCharacter* DeadPlayer);
    void FinishLevel(AdsaprojectCharacter* Player);
    void AddScore(int32 ScoreToAdd);
};