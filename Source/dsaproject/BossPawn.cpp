#include "BossPawn.h"
#include "PaperFlipbook.h"

ABossPawn::ABossPawn()
{
    PrimaryActorTick.bCanEverTick = false;
    Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
    RootComponent = Sprite;
}

void ABossPawn::BeginPlay()
{
    Super::BeginPlay();
    if (IdleAnim)
    {
        Sprite->SetFlipbook(IdleAnim);
    }
}

void ABossPawn::PlayAttack()
{
    if (AttackAnim)
    {
        Sprite->SetFlipbook(AttackAnim);
        Sprite->SetLooping(false);
        Sprite->PlayFromStart();

        float Duration = AttackAnim->GetTotalDuration();
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().SetTimer(AnimTimer, this, &ABossPawn::ReturnToIdle, Duration, false);
        }
    }
}

void ABossPawn::PlayDefend()
{
    if (DefendAnim)
    {
        Sprite->SetFlipbook(DefendAnim);
        Sprite->SetLooping(false);
        Sprite->PlayFromStart();

        float Duration = DefendAnim->GetTotalDuration();
        if (GetWorld())
        {
            GetWorld()->GetTimerManager().SetTimer(AnimTimer, this, &ABossPawn::ReturnToIdle, Duration, false);
        }
    }
}

void ABossPawn::PlayDeath()
{
    if (DeathAnim)
    {
        Sprite->SetFlipbook(DeathAnim);
        Sprite->SetLooping(false);
        Sprite->PlayFromStart();

        float Duration = DeathAnim->GetTotalDuration();
        if (GetWorld())
        {
            // Call FinishDeath instead of ReturnToIdle
            GetWorld()->GetTimerManager().SetTimer(AnimTimer, this, &ABossPawn::FinishDeath, Duration, false);
        }
    }
    else
    {
        // If no animation, hide immediately
        FinishDeath();
    }
}

void ABossPawn::ReturnToIdle()
{
    if (IdleAnim)
    {
        Sprite->SetFlipbook(IdleAnim);
        Sprite->SetLooping(true);
        Sprite->Play();
    }
}

void ABossPawn::FinishDeath()
{
    // Hide the sprite to simulate "disappearing"
    Sprite->SetVisibility(false);
}