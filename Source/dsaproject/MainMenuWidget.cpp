#include "MainMenuWidget.h"
#include "DSAGameInstance.h"
#include "SaveSlotWidget.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn_NewGame) Btn_NewGame->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
    if (Btn_LoadGame) Btn_LoadGame->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadGameListClicked);
}

void UMainMenuWidget::OnNewGameClicked()
{
    if (!Input_PlayerName) return;

    FString Name = Input_PlayerName->GetText().ToString();
    if (Name.IsEmpty()) Name = TEXT("Player");

    UDSAGameInstance* GI = Cast<UDSAGameInstance>(GetGameInstance());
    if (GI)
    {
        GI->CreateNewGame(Name);
    }
}

void UMainMenuWidget::OnLoadGameListClicked()
{
    // 1. Clear existing list
    if (VBox_SaveSlots) VBox_SaveSlots->ClearChildren();

    UDSAGameInstance* GI = Cast<UDSAGameInstance>(GetGameInstance());
    if (!GI || !SaveSlotClass) return;

    // 2. Get Slots (Using dummy data or real scan)
    TArray<FString> Slots = GI->GetAllSaveSlots();

    // 3. Create a widget for each slot
    for (FString SlotName : Slots)
    {
        USaveSlotWidget* NewSlot = CreateWidget<USaveSlotWidget>(this, SaveSlotClass);
        if (NewSlot)
        {
            NewSlot->Setup(SlotName, this);
            VBox_SaveSlots->AddChild(NewSlot);
        }
    }
}

void UMainMenuWidget::LoadSpecificSlot(FString SlotName)
{
    UDSAGameInstance* GI = Cast<UDSAGameInstance>(GetGameInstance());
    if (GI)
    {
        GI->LoadGame(SlotName);
    }
}