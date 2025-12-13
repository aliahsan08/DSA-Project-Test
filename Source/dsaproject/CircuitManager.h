#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CircuitNode.h"
#include "Containers/Queue.h" 
#include "CircuitManager.generated.h"

UCLASS()
class DSAPROJECT_API ACircuitManager : public AActor
{
    GENERATED_BODY()

public:
    ACircuitManager();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditInstanceOnly, Category = "Setup")
        ACircuitNode* GeneratorNode;

    UFUNCTION(BlueprintCallable)
        void OnNodeInteracted(ACircuitNode* Node);

    // Helper for GameMode transition check
    bool IsCircuitComplete() const { return bDoorOpen; }

private:
    void RunBFS();
    bool bDoorOpen;
};