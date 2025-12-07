#include "GameInputComponent.h"
#include "dsaprojectCharacter.h"
#include "BossManager.h"
#include "Kismet/GameplayStatics.h"

UGameInputComponent::UGameInputComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UGameInputComponent::BeginPlay()
{
    Super::BeginPlay();
    SetupInputMode(EInputMode::EM_Platformer);
}

void UGameInputComponent::SetupInputMode(EInputMode NewMode)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;
    APawn* PlayerPawn = PC->GetPawn();
    if (!PlayerPawn) return;

    UInputComponent* InputComponent = PlayerPawn->InputComponent;

    // FIX: Removed UnbindAllInputs as accessing Bindings array directly is private in UE4.27.
    // Instead we just overwrite/add new bindings.

    if (NewMode == EInputMode::EM_Platformer)
    {
        BindPlatformerInputs(InputComponent);
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
    else if (NewMode == EInputMode::EM_BattleMenu)
    {
        InputComponent->BindKey(EKeys::U, IE_Pressed, this, &UGameInputComponent::HandleBattleAttack);
        InputComponent->BindKey(EKeys::I, IE_Pressed, this, &UGameInputComponent::HandleBattleDefend);
        InputComponent->BindKey(EKeys::O, IE_Pressed, this, &UGameInputComponent::HandleBattleCharge);

        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
}

void UGameInputComponent::BindPlatformerInputs(UInputComponent* InputComponent)
{
    if (!InputComponent) return;
    InputComponent->BindAxis("MoveRight", this, &UGameInputComponent::HandleMoveRight);
    InputComponent->BindAction("Jump", IE_Pressed, this, &UGameInputComponent::HandleJump);
    InputComponent->BindAction("Jump", IE_Released, this, &UGameInputComponent::HandleStopJump);
}

void UGameInputComponent::HandleMoveRight(float Value)
{
    AdsaprojectCharacter* Character = Cast<AdsaprojectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Character) Character->MoveRight(Value); // Now works because MoveRight is Public
}

void UGameInputComponent::HandleJump()
{
    AdsaprojectCharacter* Character = Cast<AdsaprojectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Character) Character->Jump();
}

void UGameInputComponent::HandleStopJump()
{
    AdsaprojectCharacter* Character = Cast<AdsaprojectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Character) Character->StopJumping();
}

void UGameInputComponent::HandleBattleAttack()
{
    ABossManager* Manager = Cast<ABossManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossManager::StaticClass()));
    if (Manager) Manager->ExecuteTurn(EBattleAction::Attack);
}

void UGameInputComponent::HandleBattleDefend()
{
    ABossManager* Manager = Cast<ABossManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossManager::StaticClass()));
    if (Manager) Manager->ExecuteTurn(EBattleAction::Defend);
}

void UGameInputComponent::HandleBattleCharge()
{
    ABossManager* Manager = Cast<ABossManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossManager::StaticClass()));
    if (Manager) Manager->ExecuteTurn(EBattleAction::Charge);
}