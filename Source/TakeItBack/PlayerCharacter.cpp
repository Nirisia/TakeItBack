// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter() : Super()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 86.0f);

    
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
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
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
    GetCharacterMovement()->RotationRate = FRotator(0.0f, RotationSpeed, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->JumpZVelocity = JumpSpeed;
    GetCharacterMovement()->AirControl = AirControl;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    
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
    if ( (Controller != NULL) && (Value != 0.0f) )
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

void APlayerCharacter::ChangeWeapon_Implementation()
{
    if (bCanChangeWeapon)
    {
        bCanChangeWeapon = false;
        bCanAttack = false;
        AtkCount = 0;
        PlayAnimMontage(ChangeWeaponAnim, ChangeWeaponSpeed);
    }

}

void APlayerCharacter::Attack()
{
    if(bCanAttack == true)
    {
        bCanChangeWeapon = false;
        bCanAttack = false;
        if(bIsAxe == true)
        {
            PlayAnimMontage(AxeAttacksAnim[AtkCount], AtkSpeed);
        }
        else
        {
            PlayAnimMontage(SwordAttacksAnim[AtkCount], AtkSpeed);
        }
        if(AtkCount >= 4)
        {
            AtkCount = 0;
        }
        else
        {
            AtkCount++;
        } 
    }
}

void APlayerCharacter::ValidateAttack()
{
    bCanAttack = true;
}

void APlayerCharacter::ResetCombo()
{
    AtkCount = 0;
    bCanAttack = true;
    bCanChangeWeapon = true;
}


void APlayerCharacter::SpecialAttack()
{
}

void APlayerCharacter::Defense()
{
}

void APlayerCharacter::StopDefense()
{
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
    PlayerInputComponent->BindAction("Defense", IE_Released, this, &APlayerCharacter::StopDefense);

    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
    PlayerInputComponent->BindAction("SpecialAttack", IE_Pressed, this, &APlayerCharacter::SpecialAttack);
    PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &APlayerCharacter::ChangeWeapon);
}
