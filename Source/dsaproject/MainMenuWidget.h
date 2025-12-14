#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h" 
#include "Components/TextBlock.h" 
#include "MainMenuWidget.generated.h"

UCLASS()
class DSAPROJECT_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
        UEditableTextBox* Input_PlayerName;

    UPROPERTY(meta = (BindWidget))
        UButton* Btn_NewGame;

    UPROPERTY(meta = (BindWidget))
        UButton* Btn_LoadGame;

    UPROPERTY(meta = (BindWidget))
        UTextBlock* Txt_ErrorMsg;

    virtual void NativeConstruct() override;

private:
    UFUNCTION()
        void OnNewGameClicked();

    UFUNCTION()
        void OnLoadGameClicked(); // Renamed logic
};