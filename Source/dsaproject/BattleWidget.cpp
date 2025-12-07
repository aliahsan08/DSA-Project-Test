#include "BattleWidget.h"
#include "BossManager.h"
#include "Kismet/GameplayStatics.h"

void UBattleWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 1. Find the Boss Manager in the level so we can talk to it
    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABossManager::StaticClass());
    BossManagerRef = Cast<ABossManager>(FoundActor);

    if (!BossManagerRef)
    {
        UE_LOG(LogTemp, Error, TEXT("DSA UI ERROR: Could not find ABossManager in the level!"));
    }

    // 2. Bind Buttons to Functions
    // This is exactly what the "On Clicked" node does in Blueprint, but in C++
    if (Btn_Attack) Btn_Attack->OnClicked.AddDynamic(this, &UBattleWidget::OnAttackClicked);
    if (Btn_Defend) Btn_Defend->OnClicked.AddDynamic(this, &UBattleWidget::OnDefendClicked);
    if (Btn_Charge) Btn_Charge->OnClicked.AddDynamic(this, &UBattleWidget::OnChargeClicked);
}

// --- CLICK HANDLERS ---

void UBattleWidget::OnAttackClicked()
{
    if (BossManagerRef) BossManagerRef->ExecuteTurn(EBattleAction::Attack);
}

void UBattleWidget::OnDefendClicked()
{
    if (BossManagerRef) BossManagerRef->ExecuteTurn(EBattleAction::Defend);
}

void UBattleWidget::OnChargeClicked()
{
    if (BossManagerRef) BossManagerRef->ExecuteTurn(EBattleAction::Charge);
}

// --- UI UPDATERS ---

void UBattleWidget::UpdateLog(FString Message)
{
    if (Txt_BattleLog)
    {
        Txt_BattleLog->SetText(FText::FromString(Message));
    }
}

void UBattleWidget::UpdateHP(int32 PlayerHP, int32 BossHP)
{
    if (Txt_PlayerHP)
    {
        Txt_PlayerHP->SetText(FText::FromString(FString::Printf(TEXT("Player HP: %d"), PlayerHP)));
    }

    if (Txt_BossHP)
    {
        Txt_BossHP->SetText(FText::FromString(FString::Printf(TEXT("Boss HP: %d"), BossHP)));
    }
}