#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CircuitNode.generated.h"

class USphereComponent; // Forward Declaration

UCLASS()
class DSAPROJECT_API ACircuitNode : public AActor
{
    GENERATED_BODY()

public:
    ACircuitNode();

    // Visuals
    UPROPERTY(VisibleAnywhere, Category = "Visual")
        UStaticMeshComponent* Mesh;

    // Interaction Trigger
    UPROPERTY(VisibleAnywhere, Category = "Interaction")
        USphereComponent* TriggerSphere;

    // Graph Connections
    UPROPERTY(EditInstanceOnly, Category = "Graph")
        TArray<ACircuitNode*> Neighbors;

    // State
    UPROPERTY(EditAnywhere, Category = "Graph")
        bool bIsBroken;

    UPROPERTY(EditAnywhere, Category = "Graph")
        bool bIsGenerator;

    UPROPERTY(EditAnywhere, Category = "Graph")
        bool bIsDoor;

    void RepairNode();
    void UpdateColor(bool bIsPowered);

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};