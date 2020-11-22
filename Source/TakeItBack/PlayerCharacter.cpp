// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "DA_Player.h"
#include "Axe.h"
#include "Checkpoint.h"
#include "TakeItBackGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

void APlayerCharacter::LoadWeaponStats()
{
    auto Weapon = GetCurrentWeapon();
    if (Weapon)
    {
        GetCharacterMovement()->GravityScale = GravityScale * GetCurrentWeapon()->GravityScaleCoef;
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed * GetCurrentWeapon()->WalkSpeedCoef;
    }
}

void APlayerCharacter::LoadDataAssets()
{
    Super::LoadDataAssets();
    UDA_Player* PlayerData = Cast<UDA_Player>(CharacterData);
    if (PlayerData)
    {
        ChangeWeaponSpeed = PlayerData->ChangeWeaponSpeed;
        StackLimit = PlayerData->StackLimit;
        BaseTurnRate = PlayerData->BaseTurnRate;
        BaseLookUpRate = PlayerData->BaseLookUpRate;
    }
}

void APlayerCharacter::SetupStimulus()
{
    Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
    Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
    Stimulus->RegisterWithPerceptionSystem();
}

APlayerCharacter::APlayerCharacter() : Super()
{

    Axe = CreateDefaultSubobject<UChildActorComponent>(TEXT("Axe"));
    Axe->SetupAttachment(Cast<USceneComponent>(GetMesh()), "LeftWeaponShield");

    Sword = CreateDefaultSubobject<UChildActorComponent>(TEXT("Sword"));
    Sword->SetupAttachment(Cast<USceneComponent>(GetMesh()), "weaponPosition_r");
    Sword->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));

    Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
    Shield->SetupAttachment(Cast<USceneComponent>(GetMesh()), "backpackShield02");
    Shield->SetRelativeRotation(FRotator(-90.f, 0.f, -90.f));


    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
    // are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

    SetupStimulus();
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    LoadDataAssets();

    LoadWeaponStats();
    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
}

int APlayerCharacter::MyTakeDamage(int Damage, EWeaponResistance WeaponType, FVector Direction)
{
    if (!GetCurrentWeapon()->CanTakeDamage()) return 0;
    return Super::MyTakeDamage(Damage, WeaponType);
}

void APlayerCharacter::MoveForward(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void APlayerCharacter::MoveRight(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void APlayerCharacter::TurnAtRate(float Rate)
{
    UTakeItBackGameInstance* GameInstance = Cast<UTakeItBackGameInstance>(GetGameInstance());
    if (GameInstance && GameInstance->bInverseXAxis)
    {
        AddControllerYawInput(-Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());

    }
    else
    {
        AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
    }
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
    UTakeItBackGameInstance* GameInstance = Cast<UTakeItBackGameInstance>(GetGameInstance());
    if (GameInstance && GameInstance->bInverseYAxis)
    {
        AddControllerPitchInput(-Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
    }
    else
    {
        AddControllerPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
    }
}

void APlayerCharacter::ChangeWeapon()
{
    if (bCanChangeWeapon)
    {
        bCanDefend = false;
        bCanChangeWeapon = false;
        bCanSpecialAttack = false;
        bCanAttack = false;

        PlayAnimMontage(ChangeWeaponAnim, ChangeWeaponSpeed);
    }
}

void APlayerCharacter::SwapMeshes()
{
    if (bIsAxe)
    {
        bIsAxe = false;
        Axe->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));
        Axe->AttachToComponent(Axe->GetAttachParent(), FAttachmentTransformRules::KeepRelativeTransform,
                               "weaponPosition_l");

        Sword->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
        Sword->AttachToComponent(Sword->GetAttachParent(), FAttachmentTransformRules::KeepRelativeTransform,
                                 "weaponShield_r");

        Shield->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
        Shield->AttachToComponent(Shield->GetAttachParent(), FAttachmentTransformRules::KeepRelativeTransform,
                                  "weaponShield_l");
    }
    else
    {
        bIsAxe = true;
        Axe->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
        Axe->AttachToComponent(Axe->GetAttachParent(), FAttachmentTransformRules::KeepRelativeTransform,
                               "weaponShield_l");

        Sword->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));
        Sword->AttachToComponent(Sword->GetAttachParent(), FAttachmentTransformRules::KeepRelativeTransform,
                                 "weaponPosition_r");

        Shield->SetRelativeRotation(FRotator(-90.f, 0.f, -90.f));
        Shield->AttachToComponent(Shield->GetAttachParent(), FAttachmentTransformRules::KeepRelativeTransform,
                                  "backpackShield02");
    }
    LoadWeaponStats();
    bCanAttack = true;
    bCanDefend = true;
    bCanChangeWeapon = true;
    bCanSpecialAttack = true;
    OnChangeWeapon();
    
}

void APlayerCharacter::Attack()
{
    if (bCanAttack == true)
    {
        bCanDefend = false;
        bCanChangeWeapon = false;
        bCanAttack = false;
        bCanSpecialAttack = false;
        GetCurrentWeapon()->LightAttack();

        GetCharacterMovement()->MaxWalkSpeed = AttackSpeedCoeff * WalkSpeed;
        GetCharacterMovement()->RotationRate = RotationRate;
    }
}

void APlayerCharacter::ResetCombo()
{
    if (bIsDead) return;
    Super::ResetCombo();
    GetCurrentWeapon()->ResetCombo();
    bCanDefend = true;
    bCanSpecialAttack = true;
    bCanChangeWeapon = true;
    
    GetCharacterMovement()->RotationRate = RotationRate;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}


void APlayerCharacter::SpecialAttack()
{
    if (bCanSpecialAttack && GetCurrentWeapon()->Power == GetCurrentWeapon()->MaxPower)
    {
        GetCurrentWeapon()->SpecialAttack();
    }
}

void APlayerCharacter::Defense()
{
    GetCurrentWeapon()->Defense();   
}

void APlayerCharacter::StopDefense()
{
    GetCurrentWeapon()->StopDefense();
}

AWeapon* APlayerCharacter::GetCurrentWeapon()
{
    if (bIsAxe == true)
    {
        return Cast<AWeapon>(Axe->GetChildActor());
    }
    return Cast<AWeapon>(Sword->GetChildActor());
}

void APlayerCharacter::SetShieldMesh(UStaticMesh* ShieldMesh)
{
    Shield->SetStaticMesh(ShieldMesh);
}

void APlayerCharacter::SetInvulnerable(bool bInvunerable)
{
    bInvulnerable = bInvunerable;
}

void APlayerCharacter::Heal(float HealPercent)
{
    OnHeal();
    CurrentLife += MaxLife * HealPercent;
        
    if(CurrentLife > MaxLife)
    {
        CurrentLife = MaxLife;
    }
}

void APlayerCharacter::Die()
{
    Super::Die();
    bCanChangeWeapon = false;
    bCanSpecialAttack = false;
    bCanDefend = false;
}

void APlayerCharacter::Revive()
{
    Super::Revive();
    bCanChangeWeapon = true;
    bCanSpecialAttack = true;
    bCanDefend = true;
    Cast<AWeapon>(Axe->GetChildActor())->Power = 0;
    Cast<AWeapon>(Sword->GetChildActor())->Power = 0;
}

void APlayerCharacter::Respawn()
{
    if(Checkpoint)
    {
        OnRespawn();
        SetActorLocation(Checkpoint->GetRespawnLocation(), false, nullptr, ETeleportType::ResetPhysics);
        Revive();
    }
}
