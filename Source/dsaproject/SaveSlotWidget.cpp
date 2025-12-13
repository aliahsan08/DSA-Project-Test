#include "SaveSlotWidget.h"
#include "MainMenuWidget.h"

void USaveSlotWidget::NativeConstruct()
{
    Super::NativeConstruct();
    if (Btn_Slot)
    {
        Btn_Slot->OnClicked.AddDynamic(this, &USaveSlotWidget::OnClicked);
    }
}

void USaveSlotWidget::Setup(FString SlotName, UMainMenuWidget* InParentMenu)
{
    MySlotName = SlotName;
    ParentMenu = InParentMenu;

    if (Txt_SlotName)
    {
        Txt_SlotName->SetText(FText::FromString(SlotName));
    }
}

void USaveSlotWidget::OnClicked()
{
    if (ParentMenu)
    {
        // Tell the main menu to load this specific slot
        ParentMenu->LoadSpecificSlot(MySlotName);
    }
}