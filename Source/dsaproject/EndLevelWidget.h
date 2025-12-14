#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "EndLevelWidget.generated.h"

UCLASS()
class DSAPROJECT_API UEndLevelWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
        UTextBlock* Txt_Score;

    UPROPERTY(meta = (BindWidget))
        UTextBlock* Txt_HighScores;

    UPROPERTY(meta = (BindWidget))
        UButton* Btn_NextLevel;

    // NEW: Save Button
    UPROPERTY(meta = (BindWidget))
        UButton* Btn_Save;

    virtual void NativeConstruct() override;

private:
    UFUNCTION()
        void OnNextLevelClicked();

    UFUNCTION()
        void OnSaveClicked();
};