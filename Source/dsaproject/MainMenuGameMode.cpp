#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AMainMenuGameMode::AMainMenuGameMode()
{
    // We don't need a pawn for the menu
    DefaultPawnClass = nullptr;
}

void AMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuWidgetClass)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
        {
            // 1. Create the Widget
            UUserWidget* Menu = CreateWidget<UUserWidget>(PC, MainMenuWidgetClass);

            if (Menu)
            {
                // 2. Add to Screen
                Menu->AddToViewport();

                // 3. Set Input Mode (Mouse Only)
                PC->bShowMouseCursor = true;

                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(Menu->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

                PC->SetInputMode(InputMode);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("DSA ERROR: MainMenuWidgetClass is NOT set in BP_MainMenuGameMode!"));
    }
}