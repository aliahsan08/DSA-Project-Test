#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuWidget.h" // Includes the widget definition
#include "MainMenuGameMode.generated.h"

UCLASS()
class DSAPROJECT_API AMainMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMainMenuGameMode();

protected:
    virtual void BeginPlay() override;

public:
    // Assign WBP_MainMenu here in the Editor
    UPROPERTY(EditDefaultsOnly, Category = "UI")
        TSubclassOf<UUserWidget> MainMenuWidgetClass;
};