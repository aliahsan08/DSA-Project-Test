#include "InventoryComponent.h"
#include "dsaprojectGameMode.h"
#include "Kismet/GameplayStatics.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddItem(ECollectibleType ItemType, int32 Value)
{
    // Update the TMap (DSA) - O(1) average lookup/insertion
    int32 CurrentCount = Items.FindOrAdd(ItemType, 0);
    Items.Add(ItemType, CurrentCount + 1);

    // Update score through GameMode
    AdsaprojectGameMode* GameMode = Cast<AdsaprojectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    if (GameMode)
    {
        GameMode->AddScore(Value);
    }

    // --- FIX: Use StaticEnum instead of string lookup to prevent crash ---
    // This gets the readable name (e.g., "Coin") safely
    const UEnum* EnumPtr = StaticEnum<ECollectibleType>();
    FString TypeName = EnumPtr ? EnumPtr->GetNameStringByValue((int64)ItemType) : TEXT("Unknown");

    UE_LOG(LogTemp, Log, TEXT("DSA: Collected Item. Type: %s, Total: %d"), *TypeName, CurrentCount + 1);
}