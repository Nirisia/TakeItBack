// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Axe.h"
#include "Sword.h"
#include "Engine/Engine.h"

APlayerCharacter::APlayerCharacter() : Super()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 86.0f);

    Axe = CreateDefaultSubobject<UChildActorComponent>(TEXT("Axe"));
    Axe->SetupAttachment(Cast<USceneComponent>(GetMesh()), "LeftWeaponShield");

    Sword = CreateDefaultSubobject<UChildActorComponent>(TEXT("Sword"));
    Sword->SetupAttachment(Cast<USceneComponent>(GetMesh()), "weaponPosition_r");
    Sword->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));

    Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
    Shield->SetupAttachment(Cast<USceneComponent>(GetMesh()), "backpackShield02");
    Shield->SetRelativeRotation(FRotator(-90.f, 0.f, -90.f));

    SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    SphereComponent->SetupAttachment(RootComponent);
    SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    SphereComponent->SetGenerateOverlapEvents(false);


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
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    CameraBoom->TargetArmLength = 400.f; // The camera follows at this distance behind the character
    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
    GetCharacterMovement()->JumpZVelocity = JumpSpeed;
    GetCharacterMovement()->AirControl = AirControl;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, RotationSpeed, 0.0f); // ...at this rotation rate

    CameraBoom->TargetArmLength = CameraOffset.Size();
    CameraBoom->SocketOffset = FVector(0.0f, 0.f, CameraOffset.Z);

    FollowCamera->SetRelativeRotation(FRotator(CameraAngle, 0.f, 0.f));
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
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
    AddControllerPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
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
    bCanAttack = true;
    bCanDefend = true;
    bCanChangeWeapon = true;
    bCanSpecialAttack = true;
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

        GetCharacterMovement()->MaxWalkSpeed = 0.7f * WalkSpeed;
        GetCharacterMovement()->RotationRate = FRotator(0.f, 0.2f * RotationSpeed,0.f);
    }
}

void APlayerCharacter::ValidateAttack()
{
    //GetCharacterMovement()->RotationRate = FRotator(0.f, RotationSpeed, 0.f);
    bCanAttack = true;
}

void APlayerCharacter::ResetCombo()
{
    GetCurrentWeapon()->ResetCombo();
    bCanAttack = true;
    bCanDefend = true;
    bCanSpecialAttack = true;
    bCanChangeWeapon = true;
    
    GetCharacterMovement()->RotationRate = FRotator(0.f, RotationSpeed, 0.f);
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

void APlayerCharacter::SetWeaponCollision(bool bGenerateOverlap)
{
    GetCurrentWeapon()->SetWeaponCollision(bGenerateOverlap);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings
    check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

    PlayerInputComponent->BindAction("Defense", IE_Pressed, this, &APlayerCharacter::Defense);
    //PlayerInputComponent->BindAction("Defense", IE_Released, this, &APlayerCharacter::StopDefense);

    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
    PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &APlayerCharacter::SpecialAttack);
    PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &APlayerCharacter::ChangeWeapon);
}
