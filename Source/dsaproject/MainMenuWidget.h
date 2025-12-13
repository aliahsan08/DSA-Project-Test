#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
// CRITICAL FIX: Changed from EditableTextBox.h to EditableText.h
#include "Components/EditableText.h" 
#include "Components/VerticalBox.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class DSAPROJECT_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // --- UI BINDINGS ---
    // Matches the "Editable Text" widget in your UMG Designer
    UPROPERTY(meta = (BindWidget))
        UEditableText* Input_PlayerName;

    UPROPERTY(meta = (BindWidget))
        UButton* Btn_NewGame;

    UPROPERTY(meta = (BindWidget))
        UButton* Btn_LoadGame;

    UPROPERTY(meta = (BindWidget))
        UVerticalBox* VBox_SaveSlots;

    // --- CONFIG ---
    UPROPERTY(EditAnywhere, Category = "Setup")
        TSubclassOf<UUserWidget> SaveSlotClass;

    // --- LOGIC ---
    virtual void NativeConstruct() override;

    void LoadSpecificSlot(FString SlotName);

private:
    UFUNCTION()
        void OnNewGameClicked();

    UFUNCTION()
        void OnLoadGameListClicked();
};