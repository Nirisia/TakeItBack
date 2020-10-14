// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "PlayerCharacter.h"
#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"

AAxe::AAxe()
{
}

void AAxe::LightAttack()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("AxeAttack"));
}

void AAxe::SpecialAttack()
{
    FireStorm();
}

void AAxe::Tick(float DeltaTime)
{
    if (bIsFireStormActive)
    {
        if (ElapsedTime > 3.0f)
        {
            APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter);

            auto PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();

            auto PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
            PlayerController->PlayDynamicForceFeedback(UKismetMathLibrary::Cos(ElapsedTime * UKismetMathLibrary::GetPI()), -1.f,
                true, true, true, true,
                EDynamicForceFeedbackAction::Update, ForceFeedbackHandle);
            
            PlayerCharacterMovement->MaxWalkSpeed = PlayerCharacter->WalkSpeed;
            PlayerCharacterMovement->RotationRate = FRotator(0.0f, PlayerCharacter->RotationSpeed, 0.0f);

            
            PlayerCharacter->bCanAttack = true;
            PlayerCharacter->bCanSpecialAttack = true;
            PlayerCharacter->bCanChangeWeapon = true;

            MeshComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

            SetWeaponCollision(false);

            bIsFireStormActive = false;
        }
        
        OwnerCharacter->AddActorWorldRotation(FRotator(0.f, 3 * 360.f * DeltaTime, 0.f ));
        
        ElapsedTime += DeltaTime;
    }
}

void AAxe::FireStorm_Implementation()
{
    if (!bIsFireStormActive)
    {
        ElapsedTime = 0.f;
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter);
        auto PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();

        MeshComponent->SetRelativeRotation(FRotator(-80.f, -180.f, 0.f));
        
        PlayerCharacterMovement->MaxWalkSpeed = 150.f;
        PlayerCharacterMovement->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
        auto PlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
        ForceFeedbackHandle = PlayerController->PlayDynamicForceFeedback(1.0f, -1.f, true, true, true, true);

        PlayerCharacter->bCanAttack = false;
        PlayerCharacter->bCanSpecialAttack = false;
        PlayerCharacter->bCanChangeWeapon = false;

        SetWeaponCollision(true);
        
        bIsFireStormActive = true;   
    }
}
