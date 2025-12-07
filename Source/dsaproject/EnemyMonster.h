#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "EnemyMonster.generated.h"

UCLASS()
class DSAPROJECT_API AEnemyMonster : public APaperCharacter
{
    GENERATED_BODY()

public:
    AEnemyMonster();
    virtual void Tick(float DeltaTime) override;

    // Collision handlers
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "AI")
        float MoveSpeed;

    UPROPERTY(EditAnywhere, Category = "AI")
        float LedgeCheckDistance;

    // Protected: Helper variables for movement
    float PatrolDirection;

    // Protected: Functions available to Child classes
    bool CheckForLedge(FVector DirectionVector);
    void Patrol(float DeltaTime);

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};