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
    if(ControllerData)
    {
        if (PlayerCameraManager)
        {
            PlayerCameraManager->ViewPitchMin = ControllerData->ViewPitchMin;
            PlayerCameraManager->ViewPitchMax = ControllerData->ViewPitchMax;
            PlayerCameraManager->GetCameraRotation() = ControllerData->CameraRotation;
        }
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
        if (PlayerCharacter)
        {
            PlayerCharacter->GetCameraBoom()->CameraLagSpeed = ControllerData->CameraLagSpeed;
            PlayerCharacter->GetFollowCamera()->SetFieldOfView(ControllerData->CameraFOV);
        }
        TimeBeforeAutoCamera = ControllerData->TimeBeforeAutoCamera;
        BaseFOV = ControllerData->CameraFOV;
        SetControlRotation(ControllerData->InitialControllerRotation);
        AutoCameraSensitivity = ControllerData->AutoCameraSensitivity;
    }
}

void AMainPlayerController::CameraTick(float DeltaSeconds)
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
    if (PlayerCharacter)
    {
        if (GetCharacter()->GetCharacterMovement()->IsFalling() || GetInputAxisValue("LookUpRate") || GetInputAxisValue("TurnRate"))
        {
            CameraElapsedTime = 0.f;
        }
    
        if (CameraElapsedTime > TimeBeforeAutoCamera)
        {
            const float Ratio = UKismetMathLibrary::SafeDivide(PlayerCharacter->GetVelocity().Size(), PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed);
            FVector CameraForward = PlayerCharacter->GetFollowCamera()->GetForwardVector();
            CameraForward.Normalize();

            FVector ActorForward = PlayerCharacter->GetActorForwardVector();
            ActorForward.Normalize();
        
            const FVector InputVector = FVector::CrossProduct(CameraForward, ActorForward);
            const FRotator NewRotation = GetControlRotation().Add(0, InputVector.Z * Ratio * AutoCameraSensitivity, 0.f);
            SetControlRotation(NewRotation);
        }
        else
        {
            CameraElapsedTime += DeltaSeconds;
        }
    }
}

void AMainPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    CameraTick(DeltaSeconds);
}

AMainPlayerController::AMainPlayerController() : Super()
{
    RootComponent->SetUsingAbsoluteRotation(true);
}

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();
    SetupInputComponent();
    LoadDataAssets();
}

void AMainPlayerController::SetupInputComponent()
{    
    Super::SetupInputComponent();
    // Set up gameplay key bindings
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
    if (PlayerCharacter && InputComponent)
    {
        InputComponent->BindAction("Jump", IE_Pressed, PlayerCharacter, &APlayerCharacter::Jump);
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

        InputComponent->BindAction("Defense", IE_Pressed, PlayerCharacter, &APlayerCharacter::Defense);
        //PlayerInputComponent->BindAction("Defense", IE_Released, this, &APlayerCharacter::StopDefense);

        InputComponent->BindAction("Attack", IE_Pressed, PlayerCharacter, &APlayerCharacter::Attack);
        InputComponent->BindAction("SpecialAttack", IE_Pressed, PlayerCharacter, &APlayerCharacter::SpecialAttack);
        InputComponent->BindAction("ChangeWeapon", IE_Pressed, PlayerCharacter, &APlayerCharacter::ChangeWeapon);
    }
}
