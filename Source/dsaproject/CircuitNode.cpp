#include "CircuitNode.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h" // Needed for the trigger
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "CircuitManager.h" // Needed to call OnNodeInteracted
#include "dsaprojectCharacter.h" // Needed to check if it's the player

ACircuitNode::ACircuitNode()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    // Create the Trigger Sphere (larger than the mesh so it's easy to hit)
    TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
    TriggerSphere->SetupAttachment(RootComponent);
    TriggerSphere->SetSphereRadius(100.0f);
    TriggerSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMesh.Succeeded())
    {
        Mesh->SetStaticMesh(CubeMesh.Object);
    }

    bIsBroken = true;
    bIsGenerator = false;
    bIsDoor = false;
}

void ACircuitNode::BeginPlay()
{
    Super::BeginPlay();

    // Bind the overlap event
    if (TriggerSphere)
    {
        TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ACircuitNode::OnOverlapBegin);
    }

    UpdateColor(false);
}

void ACircuitNode::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if the actor touching us is the Player
    AdsaprojectCharacter* Player = Cast<AdsaprojectCharacter>(OtherActor);

    if (Player)
    {
        // Find the Manager in the level
        ACircuitManager* Manager = Cast<ACircuitManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ACircuitManager::StaticClass()));

        if (Manager)
        {
            // Tell the manager to handle the logic (Fix node + Run BFS)
            Manager->OnNodeInteracted(this);
            UE_LOG(LogTemp, Log, TEXT("DSA: Player touched node: %s"), *GetName());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("DSA ERROR: CircuitManager NOT FOUND in Level!"));
        }
    }
}

void ACircuitNode::RepairNode()
{
    UE_LOG(LogTemp, Warning, TEXT("DSA: RepairNode called on %s. Old Broken State: %d"), *GetName(), bIsBroken);

    // FIX: One-way repair only. Do not toggle back to broken.
    if (bIsBroken)
    {
        bIsBroken = false;
    }
}

void ACircuitNode::UpdateColor(bool bIsPowered)
{
    FLinearColor Color = FLinearColor::Gray; // Default for Fixed but Unpowered

    if (bIsGenerator) Color = FLinearColor::Blue;
    else if (bIsDoor) Color = FLinearColor::Yellow;
    else if (bIsBroken) Color = FLinearColor::Red;
    else if (bIsPowered) Color = FLinearColor::Green;

    UMaterialInstanceDynamic* DynMaterial = Cast<UMaterialInstanceDynamic>(Mesh->GetMaterial(0));

    if (DynMaterial)
    {
        DynMaterial->SetVectorParameterValue(FName(TEXT("NodeColor")), Color);
    }
    else
    {
        if (Mesh->GetStaticMesh())
        {
            UMaterialInterface* CurrentMaterial = Mesh->GetMaterial(0);
            if (CurrentMaterial)
            {
                DynMaterial = UMaterialInstanceDynamic::Create(CurrentMaterial, this);
                if (DynMaterial)
                {
                    Mesh->SetMaterial(0, DynMaterial);
                    DynMaterial->SetVectorParameterValue(FName(TEXT("NodeColor")), Color);
                    UE_LOG(LogTemp, Log, TEXT("DSA: Created Dynamic Material for %s. Setting Color: %s"), *GetName(), *Color.ToString());
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("DSA ERROR: Failed to create Dynamic Material for %s! Check if Material allows it."), *GetName());
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("DSA ERROR: No Material assigned to Mesh on %s! Assign M_NodeColor in Blueprint."), *GetName());
            }
        }
    }
}