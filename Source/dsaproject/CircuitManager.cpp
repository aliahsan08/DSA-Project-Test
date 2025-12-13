#include "CircuitManager.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ACircuitManager::ACircuitManager()
{
    PrimaryActorTick.bCanEverTick = true;
    bDoorOpen = false;
}

void ACircuitManager::BeginPlay()
{
    Super::BeginPlay();
    RunBFS();
}

void ACircuitManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!bDoorOpen)
    {
        RunBFS();
    }
}

void ACircuitManager::OnNodeInteracted(ACircuitNode* Node)
{
    if (Node && !Node->bIsGenerator && !Node->bIsDoor)
    {
        Node->RepairNode();
        RunBFS();
    }
}

void ACircuitManager::RunBFS()
{
    if (!GeneratorNode) return;

    // RESET COLORS FIRST
    TArray<AActor*> AllNodes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACircuitNode::StaticClass(), AllNodes);
    for (AActor* Actor : AllNodes)
    {
        if (ACircuitNode* Node = Cast<ACircuitNode>(Actor))
        {
            Node->UpdateColor(false);
        }
    }

    // BFS ALGORITHM
    TQueue<ACircuitNode*> Queue;
    TSet<ACircuitNode*> Visited;

    Queue.Enqueue(GeneratorNode);
    Visited.Add(GeneratorNode);

    bool bReachedDoor = false;

    ACircuitNode* CurrentNode = nullptr;
    while (Queue.Dequeue(CurrentNode))
    {
        CurrentNode->UpdateColor(true);

        if (CurrentNode->bIsDoor)
        {
            bReachedDoor = true;
        }

        for (ACircuitNode* Neighbor : CurrentNode->Neighbors)
        {
            if (Neighbor && !Visited.Contains(Neighbor))
            {
                if (!Neighbor->bIsBroken || Neighbor->bIsDoor)
                {
                    Visited.Add(Neighbor);
                    Queue.Enqueue(Neighbor);
                    DrawDebugLine(GetWorld(), CurrentNode->GetActorLocation(), Neighbor->GetActorLocation(), FColor::Green, false, -1, 0, 5.0f);
                }
                else
                {
                    DrawDebugLine(GetWorld(), CurrentNode->GetActorLocation(), Neighbor->GetActorLocation(), FColor::Red, false, -1, 0, 2.0f);
                }
            }
        }
    }

    if (bReachedDoor && !bDoorOpen)
    {
        bDoorOpen = true;
        UE_LOG(LogTemp, Warning, TEXT("BFS COMPLETE: Circuit Closed! Door Opening."));
    }
}