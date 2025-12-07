#include "BattlePlayer.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BossManager.h" 

ABattlePlayer::ABattlePlayer()
{
    GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
    GetCapsuleComponent()->SetCapsuleRadius(40.0f);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 500.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->SetUsingAbsoluteRotation(true);
    CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

    SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
    SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
    SideViewCameraComponent->OrthoWidth = 2048.0f;
    SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

    bIsAnimating = false;
}

void ABattlePlayer::BeginPlay()
{
    Super::BeginPlay();
    if (IdleAnimation)
    {
        GetSprite()->SetFlipbook(IdleAnimation);
    }
}

void ABattlePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindKey(EKeys::U, IE_Pressed, this, &ABattlePlayer::InputAttack);
    PlayerInputComponent->BindKey(EKeys::I, IE_Pressed, this, &ABattlePlayer::InputDefend);
    PlayerInputComponent->BindKey(EKeys::O, IE_Pressed, this, &ABattlePlayer::InputCharge);
}

void ABattlePlayer::InputAttack()
{
    ABossManager* Manager = Cast<ABossManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossManager::StaticClass()));
    if (Manager) Manager->ExecuteTurn(EBattleAction::Attack);
}

void ABattlePlayer::InputDefend()
{
    ABossManager* Manager = Cast<ABossManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossManager::StaticClass()));
    if (Manager) Manager->ExecuteTurn(EBattleAction::Defend);
}

void ABattlePlayer::InputCharge()
{
    ABossManager* Manager = Cast<ABossManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossManager::StaticClass()));
    if (Manager) Manager->ExecuteTurn(EBattleAction::Charge);
}

void ABattlePlayer::PlayAttackAnim()
{
    if (AttackAnimation && !bIsAnimating)
    {
        bIsAnimating = true;
        GetSprite()->SetFlipbook(AttackAnimation);
        GetSprite()->SetLooping(false);
        GetSprite()->PlayFromStart();

        float Duration = AttackAnimation->GetTotalDuration();
        GetWorldTimerManager().SetTimer(AnimationTimer, this, &ABattlePlayer::ReturnToIdle, Duration, false);
    }
}

void ABattlePlayer::PlayDefendAnim()
{
    if (DefendAnimation && !bIsAnimating)
    {
        bIsAnimating = true;
        GetSprite()->SetFlipbook(DefendAnimation);
        GetSprite()->SetLooping(false);
        GetSprite()->PlayFromStart();

        float Duration = DefendAnimation->GetTotalDuration();
        GetWorldTimerManager().SetTimer(AnimationTimer, this, &ABattlePlayer::ReturnToIdle, Duration, false);
    }
}

void ABattlePlayer::ReturnToIdle()
{
    bIsAnimating = false;
    if (IdleAnimation)
    {
        GetSprite()->SetFlipbook(IdleAnimation);
        GetSprite()->SetLooping(true);
        GetSprite()->Play();
    }
}