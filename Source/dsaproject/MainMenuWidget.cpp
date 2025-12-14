#include "MainMenuWidget.h"
#include "DSAGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_NewGame) Btn_NewGame->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
    if (Btn_LoadGame) Btn_LoadGame->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadGameClicked);

    if (Txt_ErrorMsg) Txt_ErrorMsg->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuWidget::OnNewGameClicked()
{
    if (!Input_PlayerName) return;

    FString Name = Input_PlayerName->GetText().ToString();
    if (Name.IsEmpty())
    {
        if (Txt_ErrorMsg)
        {
            Txt_ErrorMsg->SetText(FText::FromString("Name cannot be empty!"));
            Txt_ErrorMsg->SetVisibility(ESlateVisibility::Visible);
        }
        return;
    }

    UDSAGameInstance* GI = Cast<UDSAGameInstance>(GetGameInstance());
    if (GI)
    {
        bool bSuccess = GI->CreateNewGame(Name);
        if (!bSuccess && Txt_ErrorMsg)
        {
            Txt_ErrorMsg->SetText(FText::FromString("Name already taken!"));
            Txt_ErrorMsg->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UMainMenuWidget::OnLoadGameClicked()
{
    if (!Input_PlayerName) return;

    FString Name = Input_PlayerName->GetText().ToString();
    if (Name.IsEmpty())
    {
        if (Txt_ErrorMsg)
        {
            Txt_ErrorMsg->SetText(FText::FromString("Enter name to load!"));
            Txt_ErrorMsg->SetVisibility(ESlateVisibility::Visible);
        }
        return;
    }

    UDSAGameInstance* GI = Cast<UDSAGameInstance>(GetGameInstance());
    if (GI)
    {
        bool bLoaded = GI->LoadGame(Name);
        if (!bLoaded && Txt_ErrorMsg)
        {
            Txt_ErrorMsg->SetText(FText::FromString("Save file not found!"));
            Txt_ErrorMsg->SetVisibility(ESlateVisibility::Visible);
        }
    }
}