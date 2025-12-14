#include "dsaprojectGameMode.h"
#include "dsaprojectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "DSAManagerComponent.h" 
#include "DSAGameInstance.h"
#include "CircuitManager.h" 

AdsaprojectGameMode::AdsaprojectGameMode()
{
    DefaultPawnClass = AdsaprojectCharacter::StaticClass();
    PrimaryActorTick.bCanEverTick = true;

    DSAManager = CreateDefaultSubobject<UDSAManagerComponent>(TEXT("DSAManagerComponent"));

    CurrentScore = 700;
    CoinsCollectedInLevel = 0;
    TimeElapsed = 0.0f;
}

void AdsaprojectGameMode::BeginPlay()
{
    Super::BeginPlay();
    AActor* PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
    if (PlayerStart) SpawnLocation = PlayerStart->GetActorLocation();

    // --- CRITICAL FIX 1: RESET INPUT TO GAMEPLAY ---
    // When coming from the Main Menu (which is UI Only), the controller is stuck.
    // We force it back to Game Only so WASD/Arrows work.
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        FInputModeGameOnly GameInput;
        PC->SetInputMode(GameInput);
        PC->bShowMouseCursor = false; // Hide mouse for gameplay
    }
}

void AdsaprojectGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeElapsed += DeltaTime;

    int32 TimePenalty = FMath::FloorToInt(TimeElapsed) * 10;
    int32 RawScore = BaseScore - TimePenalty;
    if (TimeElapsed >= 60.0f || RawScore < 100)
    {
        RawScore = 100;
    }

    CurrentScore = RawScore + (CoinsCollectedInLevel * 100);
}

void AdsaprojectGameMode::AddScore(int32 ScoreToAdd)
{
    CoinsCollectedInLevel += ScoreToAdd;
}

void AdsaprojectGameMode::RespawnPlayer(AdsaprojectCharacter* DeadPlayer)
{
    if (DeadPlayer)
    {
        if (DSAManager) DSAManager->ClearHistory();

        FString CurrentLevelName = GetWorld()->GetMapName();
        CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
        UGameplayStatics::OpenLevel(GetWorld(), FName(*CurrentLevelName));
    }
}

void AdsaprojectGameMode::FinishLevel(AdsaprojectCharacter* Player)
{
    // Puzzle Check
    ACircuitManager* CircuitMgr = Cast<ACircuitManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACircuitManager::StaticClass()));
    if (CircuitMgr)
    {
        if (!CircuitMgr->IsCircuitComplete())
        {
            UE_LOG(LogTemp, Warning, TEXT("Level Finished Failed: Circuit not complete!"));
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("LEVEL COMPLETE! Final Score: %d"), CurrentScore);

    // Save Data
    UDSAGameInstance* GI = Cast<UDSAGameInstance>(GetGameInstance());
    if (GI)
    {
        if (DSAManager) DSAManager->AddScore(GI->CurrentProfileName, CurrentScore);

        GI->TotalGlobalCoins += CoinsCollectedInLevel;
        GI->CurrentLevelIndex++;

        // REMOVED: GI->SaveState(); 
        // We now wait for the user to press "Save" on the UI.
    }

    // --- CRITICAL FIX 2: SPAWN END MENU & ENABLE MOUSE ---
    if (EndLevelWidgetClass)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
        {
            UUserWidget* EndWidget = CreateWidget<UUserWidget>(PC, EndLevelWidgetClass);
            if (EndWidget)
            {
                EndWidget->AddToViewport();

                // Switch Input to UI so the player can click "Next Level"
                PC->bShowMouseCursor = true;
                FInputModeUIOnly UIInput;
                UIInput.SetWidgetToFocus(EndWidget->TakeWidget());
                PC->SetInputMode(UIInput);

                // Pause game logic (optional, stops timer)
                // UGameplayStatics::SetGamePaused(GetWorld(), true);
            }
        }
    }
}