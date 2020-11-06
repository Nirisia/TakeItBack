// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "PlayerCharacter.h"
#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "DA_Axe.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyCharacter.h"
#include "MainPlayerController.h"
#include "Engine/Engine.h"

AAxe::AAxe() : Super() {}

void AAxe::SpecialAttack()
{
    FireStorm();
    Power = 0;

    if (BonusStack < 8)
    {
        BonusStack++;
    }
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

void AAxe::AttackCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor == GetParentActor()) return;
    
    Super::AttackCollision(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    if(bIsSpecialAttackActive)
    {
        if (OtherActor->ActorHasTag("Enemy") || OtherActor->ActorHasTag("Player"))
        {
            ABaseCharacter* Enemy = Cast<ABaseCharacter>(OtherActor);
            if (IsValid(Enemy))
            {
                Enemy->MyTakeDamage(Damage, WeaponType);
            }
        }
    }
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
    }
}

void AAxe::RollEnd()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
    UCharacterMovementComponent* PlayerMovement = PlayerCharacter->GetCharacterMovement();
    PlayerMovement->MaxWalkSpeed = PlayerCharacter->WalkSpeed;
    PlayerMovement->RotationRate = PlayerCharacter->RotationRate;
    PlayerMovement->SetJumpAllowed(true);
    PlayerCharacter->bCanAttack = true;
    PlayerCharacter->bCanChangeWeapon = true;
    PlayerCharacter->bCanSpecialAttack = true;
    PlayerCharacter->bCanDefend = true;
    
}

void AAxe::Tick(float DeltaTime)
{
    if (bIsSpecialAttackActive)
    {
        if (ElapsedTime > FireStormTime)
        {
            APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());

            auto PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();

            auto PlayerController = Cast<AMainPlayerController>(PlayerCharacter->GetController());
            PlayerController->PlayDynamicForceFeedback(
                UKismetMathLibrary::Cos(ElapsedTime * UKismetMathLibrary::GetPI()), -1.f, true, true, true, true,
                EDynamicForceFeedbackAction::Update, ForceFeedbackHandle);

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
        }

        GetParentCharacter()->AddActorWorldRotation(FRotator(0.f, FireStormTurnRate * DeltaTime, 0.f));

        ElapsedTime += DeltaTime;
    }
}

void AAxe::FireStorm_Implementation()
{
    if (!bIsSpecialAttackActive)
    {
        ElapsedTime = 0.f;
        APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
        auto PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();

        MeshComponent->SetRelativeRotation(FRotator(-80.f, -180.f, 0.f));
        PlayerCharacterMovement->SetJumpAllowed(false);

        PlayerCharacterMovement->MaxWalkSpeed = FireStormWalkSpeedCoef * PlayerCharacter->WalkSpeed;
        PlayerCharacterMovement->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
        auto PlayerController = Cast<APlayerController>(GetParentCharacter()->GetController());
        ForceFeedbackHandle = PlayerController->PlayDynamicForceFeedback(1.0f, -1.f, true, true, true, true);

        PlayerCharacter->bCanDefend = false;
        PlayerCharacter->bCanAttack = false;
        PlayerCharacter->bCanSpecialAttack = false;
        PlayerCharacter->bCanChangeWeapon = false;

        PlayerCharacter->GetFollowCamera()->SetFieldOfView(ActiveFOV);

        SetWeaponCollision(true);

        bIsSpecialAttackActive = true;
    }
}
