#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SaveSlotWidget.generated.h"

// Forward declaration
class UMainMenuWidget;

UCLASS()
class DSAPROJECT_API USaveSlotWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
        UButton* Btn_Slot;

    UPROPERTY(meta = (BindWidget))
        UTextBlock* Txt_SlotName;

    // --- FIX: Added this declaration because it is used in the .cpp ---
    virtual void NativeConstruct() override;

    // Setup function to initialize this row
    void Setup(FString SlotName, UMainMenuWidget* InParentMenu);

private:
    FString MySlotName;
    UMainMenuWidget* ParentMenu;

    UFUNCTION()
        void OnClicked();
};