#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

// Simple Enum for Collectible Types (DSA Key for TMap)
UENUM(BlueprintType)
enum class ECollectibleType : uint8
{
    ECT_Coin UMETA(DisplayName = "Coin"),
    ECT_Gem UMETA(DisplayName = "Gem"),
    ECT_Key UMETA(DisplayName = "Key")
};

UCLASS(ClassGroup = (DSA), meta = (BlueprintSpawnableComponent))
class DSAPROJECT_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    /** DSA: TMap for fast lookup and capacity-limited collections (ItemType -> Count) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DSA|Inventory")
        TMap<ECollectibleType, int32> Items;

    /** Adds an item to the inventory and updates score via GameMode. */
    void AddItem(ECollectibleType ItemType, int32 Value);
};