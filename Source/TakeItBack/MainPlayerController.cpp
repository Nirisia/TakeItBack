// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"


#include "DA_PlayerController.h"
#include "PlayerCharacter.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "EnemyCharacter.h"

void AMainPlayerController::LoadDataAssets()
{
    if (ControllerData)
    {
        if (PlayerCameraManager)
        {
            PlayerCameraManager->ViewPitchMin = ControllerData->ViewPitchMin;
            PlayerCameraManager->ViewPitchMax = ControllerData->ViewPitchMax;
            PlayerCameraManager->GetCameraRotation() = ControllerData->CameraRotation;
        }
        if (PlayerCharacter)
        {
            PlayerCharacter->GetCameraBoom()->CameraLagSpeed = ControllerData->CameraLagSpeed;
            PlayerCharacter->GetFollowCamera()->SetFieldOfView(ControllerData->CameraFOV);
        }
        TimeBeforeAutoCamera = ControllerData->TimeBeforeAutoCamera;
        BaseFOV = ControllerData->CameraFOV;
        AutoCameraSensitivity = ControllerData->AutoCameraSensitivity;
    }
}

void AMainPlayerController::CameraTick(float DeltaSeconds)
{
    if (PlayerCharacter)
    {
        if (GetCharacter()->GetCharacterMovement()->IsFalling() || GetInputAxisValue("LookUpRate") ||
            GetInputAxisValue("TurnRate"))
        {
            CameraElapsedTime = 0.f;
        }

        if (CameraElapsedTime > TimeBeforeAutoCamera)
        {
            const float Ratio = UKismetMathLibrary::SafeDivide(PlayerCharacter->GetVelocity().Size(),
                                                               PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed);
            FVector CameraForward = PlayerCharacter->GetFollowCamera()->GetForwardVector();
            CameraForward.Normalize();

            FVector ActorForward = PlayerCharacter->GetActorForwardVector();
            ActorForward.Normalize();

            const FVector InputVector = FVector::CrossProduct(CameraForward, ActorForward);
            const FRotator NewRotation = GetControlRotation().
                Add(0, InputVector.Z * Ratio * AutoCameraSensitivity, 0.f);
            SetControlRotation(NewRotation);
        }
        else
        {
            CameraElapsedTime += DeltaSeconds;
        }
    }
}

void AMainPlayerController::InvalidateInput()
{
    InputBuffer.bIsValid = false;
}

void AMainPlayerController::BufferInput(EInputType InputType)
{
    InputBuffer.Type = InputType;
    InputBuffer.bIsValid = true;
}

void AMainPlayerController::Attack()
{
    if (PlayerCharacter && PlayerCharacter->bCanAttack)
    {
        PlayerCharacter->Attack();
        if (InputBuffer.Type == EInputType::Attack && InputBuffer.bIsValid == true) InputBuffer.bIsValid = false;
    }
    else
    {
        InputBuffer.Type = EInputType::Attack;
        InputBuffer.bIsValid = true;
    }
}

void AMainPlayerController::SpecialAttack()
{
    if (PlayerCharacter && PlayerCharacter->bCanSpecialAttack)
    {
        PlayerCharacter->SpecialAttack();
        if (InputBuffer.Type == EInputType::SpecialAttack && InputBuffer.bIsValid == true) InputBuffer.bIsValid = false;
    }
    else
    {
        InputBuffer.Type = EInputType::SpecialAttack;
        InputBuffer.bIsValid = true;
    }
}

void AMainPlayerController::ChangeWeapon()
{
    if (PlayerCharacter && PlayerCharacter->bCanChangeWeapon)
    {
        PlayerCharacter->ChangeWeapon();
        if (InputBuffer.Type == EInputType::ChangeWeapon && InputBuffer.bIsValid == true) InputBuffer.bIsValid = false;
    }
    else
    {
        InputBuffer.Type = EInputType::ChangeWeapon;
        InputBuffer.bIsValid = true;
    }
}

void AMainPlayerController::Defense()
{
    if (PlayerCharacter && PlayerCharacter->bCanDefend)
    {
        PlayerCharacter->Defense();
        if (InputBuffer.Type == EInputType::Defense && InputBuffer.bIsValid == true) InputBuffer.bIsValid = false;
    }
    else
    {
        InputBuffer.Type = EInputType::Defense;
        InputBuffer.bIsValid = true;
    }
}

void AMainPlayerController::Jump()
{
    if (PlayerCharacter && PlayerCharacter->CanJump())
    {
        PlayerCharacter->Jump();
        if (InputBuffer.Type == EInputType::Jump && InputBuffer.bIsValid == true) InputBuffer.bIsValid = false;
    }
    else
    {
        InputBuffer.Type = EInputType::Jump;
        InputBuffer.bIsValid = true;
    }
}

void AMainPlayerController::TryConsumeInput()
{
    if (InputBuffer.bIsValid == true)
    {
        switch (InputBuffer.Type)
        {
        case EInputType::Attack:
            Attack();
            return;
        case EInputType::SpecialAttack:
            SpecialAttack();
            return;
        case EInputType::Defense:
            Defense();
            return;
        case EInputType::ChangeWeapon:
            ChangeWeapon();
            return;
        case EInputType::Jump:
            Jump();
            return;
        case None:
        default:
            return;
        }
    }
}

void AMainPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    CameraTick(DeltaSeconds);
    if (!PlayerCharacter)
    {
        PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
        SetupInputComponent();
    }
    if (InputBuffer.bIsValid)
    {
        TryConsumeInput();
    }
}

AMainPlayerController::AMainPlayerController() : Super()
{
    RootComponent->SetUsingAbsoluteRotation(true);
}

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();
    PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
    SetupInputComponent();
    LoadDataAssets();
    SetControlRotation(GetControlRotation() + InitialControllerRotation);
}

void AMainPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    // Set up gameplay key bindings
    if (PlayerCharacter && InputComponent)
    {
        InputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayerController::Jump);
        InputComponent->BindAction("Jump", IE_Released, PlayerCharacter, &APlayerCharacter::StopJumping);

        InputComponent->BindAxis("MoveForward", PlayerCharacter, &APlayerCharacter::MoveForward);
        InputComponent->BindAxis("MoveRight", PlayerCharacter, &APlayerCharacter::MoveRight);

        // We have 2 versions of the rotation bindings to handle different kinds of devices differently
        // "turn" handles devices that provide an absolute delta, such as a mouse.
        // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
        InputComponent->BindAxis("Turn", PlayerCharacter, &APawn::AddControllerYawInput);
        InputComponent->BindAxis("TurnRate", PlayerCharacter, &APlayerCharacter::TurnAtRate);
        InputComponent->BindAxis("LookUp", PlayerCharacter, &APawn::AddControllerPitchInput);
        InputComponent->BindAxis("LookUpRate", PlayerCharacter, &APlayerCharacter::LookUpAtRate);

        InputComponent->BindAction("Defense", IE_Pressed, this, &AMainPlayerController::Defense);
        //PlayerInputComponent->BindAction("Defense", IE_Released, this, &APlayerCharacter::StopDefense);

        InputComponent->BindAction("Attack", IE_Pressed, this, &AMainPlayerController::Attack);
        InputComponent->BindAction("SpecialAttack", IE_Pressed, this, &AMainPlayerController::SpecialAttack);
        InputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &AMainPlayerController::ChangeWeapon);
    }
}
