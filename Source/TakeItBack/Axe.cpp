// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "PlayerCharacter.h"
#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "DA_Axe.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemySpawner.h"
#include "MainPlayerController.h"

AAxe::AAxe() : Super() {}

void AAxe::SpecialAttack()
{
    FireStorm();
    Power = 0;
}

void AAxe::Defense()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
    if (PlayerCharacter->bCanDefend && !PlayerCharacter->GetCharacterMovement()->IsFalling())
    {
        Super::Defense();
        Roll();
    }
}

void AAxe::LoadPower(int InflictedDamage)
{
    if (bIsSpecialAttackActive) return;
    Super::LoadPower(InflictedDamage);
}

int AAxe::GetCurrentDamage()
{
    return (bIsSpecialAttackActive)? FireStormDamage : Super::GetCurrentDamage();
}

void AAxe::Roll_Implementation() {}

void AAxe::RollStart()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
    UCharacterMovementComponent* PlayerMovement = PlayerCharacter->GetCharacterMovement();
    PlayerMovement->MaxWalkSpeed = 0;
    PlayerMovement->RotationRate = (FRotator(0));
    PlayerMovement->SetJumpAllowed(false);
    PlayerCharacter->PlayAnimMontage(RollAnim);
}

void AAxe::RollTick(float Speed)
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
    FVector NewSpeed = RollMaxSpeed * Speed * PlayerCharacter->GetActorForwardVector();
    NewSpeed.Z += PlayerCharacter->GetVelocity().Z;
    PlayerCharacter->GetCharacterMovement()->Velocity = NewSpeed;
}

void AAxe::LoadDataAssets()
{
    Super::LoadDataAssets();
    UDA_Axe* AxeData = Cast<UDA_Axe>(WeaponData);
    if (AxeData)
    {
        FireStormTime = AxeData->FireStormTime;
        FireStormTurnRate = AxeData->FireStormTurnRate;
        FireStormWalkSpeedCoef = AxeData->FireStormWalkSpeedCoef;
        ActiveFOV = AxeData->ActiveFOV;
        RollMaxSpeed = AxeData->RollMaxSpeed;
        FireStormDamage = AxeData->FireStormDamage;
    }
}

void AAxe::RollEnd()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
    UCharacterMovementComponent* PlayerMovement = PlayerCharacter->GetCharacterMovement();
    StopDefense();
    PlayerMovement->SetJumpAllowed(true);
}

void AAxe::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsSpecialAttackActive)
    {
        if (ElapsedTime > FireStormTime)
        {
            APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());

            auto PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();

            OnFireStormEnd();

            auto PlayerController = Cast<AMainPlayerController>(PlayerCharacter->GetController());

            PlayerCharacterMovement->MaxWalkSpeed = PlayerCharacter->WalkSpeed;
            PlayerCharacterMovement->RotationRate = PlayerCharacter->RotationRate;
            PlayerCharacterMovement->SetJumpAllowed(true);


            PlayerCharacter->bCanDefend = true;
            PlayerCharacter->bCanAttack = true;
            PlayerCharacter->bCanSpecialAttack = true;
            PlayerCharacter->bCanChangeWeapon = true;


            PlayerCharacter->GetFollowCamera()->SetFieldOfView(PlayerController->BaseFOV);

            MeshComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

            SetWeaponCollision(false);

            bIsSpecialAttackActive = false;
            return;
        }

        OnFireStormTick();
        GetParentCharacter()->AddActorWorldRotation(FRotator(0.f, FireStormTurnRate * DeltaTime, 0.f));

        ElapsedTime += DeltaTime;
    }
}

void AAxe::FireStorm_Implementation()
{
    if (!bIsSpecialAttackActive)
    {
        bIsSpecialAttackActive = true;
        ElapsedTime = 0.f;
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
        OnFireStormStart();
        auto PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();

        MeshComponent->SetRelativeRotation(FRotator(-80.f, -180.f, 0.f));
        PlayerCharacterMovement->SetJumpAllowed(false);

        PlayerCharacterMovement->MaxWalkSpeed = FireStormWalkSpeedCoef * PlayerCharacter->WalkSpeed;
        PlayerCharacterMovement->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
        PlayerCharacter->bCanDefend = false;
        PlayerCharacter->bCanAttack = false;
        PlayerCharacter->bCanSpecialAttack = false;
        PlayerCharacter->bCanChangeWeapon = false;

        PlayerCharacter->GetFollowCamera()->SetFieldOfView(ActiveFOV);

        SetWeaponCollision(true);

    }
}
