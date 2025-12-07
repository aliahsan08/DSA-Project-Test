#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryComponent.h"
#include "Collectible.generated.h"

class UBoxComponent;
class UPaperSpriteComponent;

UCLASS()
class DSAPROJECT_API ACollectible : public AActor
{
    GENERATED_BODY()

public:
    ACollectible();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectible")
        UBoxComponent* CollisionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectible")
        UPaperSpriteComponent* SpriteComp;

    UPROPERTY(EditAnywhere, Category = "Collectible")
        ECollectibleType ItemType;

    UPROPERTY(EditAnywhere, Category = "Collectible")
        int32 ScoreValue;

private:
    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};