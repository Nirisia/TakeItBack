// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "BaseCharacter.h"
#include "Engine.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

ASword::ASword() : Super()
{
}

void ASword::LightAttack()
{
    if (AtkCount >= AttacksAnim.Num())
    {
        AtkCount = 0;
    }

    GetParentCharacter()->PlayAnimMontage(AttacksAnim[AtkCount], AtkSpeed + 
    BonusStack * SpeedBonus * AtkSpeed);
    AtkCount++;
}

void ASword::SpecialAttack()
{
    ShieldMeteor();
}

void ASword::Defense()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
    if (PlayerCharacter->bCanDefend)
    {
        Super::Defense();
        GetParentCharacter()->GetCharacterMovement()->MaxWalkSpeed = 0.f;
        GetParentCharacter()->PlayAnimMontage(ShieldAnim);
    }
}

void ASword::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
    if (PlayerCharacter)
    {
        if (!GetParentCharacter()->GetCharacterMovement()->IsFalling())
        {
            if (bIsLaunched)
            {
                Cast<APlayerController>(GetParentCharacter()->GetController())->PlayDynamicForceFeedback(
                    1.0f, 0.5f, true, true, true, true);
                bIsLaunched = false;
                PlayerCharacter->bCanAttack = true;
                PlayerCharacter->bCanChangeWeapon = true;
                PlayerCharacter->bCanDefend = true;
                PlayerCharacter->SphereComponent->SetGenerateOverlapEvents(false);
            }
            else
            {
                bIsSpecialAttackActive = false;
            }
        }
        else if (bIsSpecialAttackActive && PlayerCharacter->GetVelocity().Z > 0)
        {
            UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
            PlayerCharacter->GetFollowCamera()->SetFieldOfView(80.f);
        }
        else
        {
            UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
            PlayerCharacter->GetFollowCamera()->SetFieldOfView(90.f);
            bIsSpecialAttackActive = false;
            PlayerCharacter->bCanAttack = true;
            PlayerCharacter->bCanChangeWeapon = true;
            PlayerCharacter->bCanDefend = true;
        }
    }
}

void ASword::ShieldMeteor_Implementation()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter()))
    {
        if (!bIsLaunched && bIsSpecialAttackActive)
        {
            FVector LaunchVelocity = PlayerCharacter->GetFollowCamera()->GetForwardVector();
            if (LaunchVelocity.Z > 0)
            {
                LaunchVelocity.Z = 0;
            }
            LaunchVelocity *= 1000;
            PlayerCharacter->LaunchCharacter(LaunchVelocity, true, true);
            PlayerCharacter->SphereComponent->SetGenerateOverlapEvents(true);
            bIsSpecialAttackActive = false;
            bIsLaunched = true;
            Power = 0;

            if(BonusStack < 8)
            {
                BonusStack++;
            }     
        }
        else if (!PlayerCharacter->GetCharacterMovement()->IsFalling())
        {
            Cast<APlayerController>(GetParentCharacter()->GetController())->PlayDynamicForceFeedback(
                    0.05f, 0.3f, false, true, false, true);
            PlayerCharacter->LaunchCharacter(FVector(0.f, 0.f, 1000.f), true, true);
            PlayerCharacter->bCanAttack = false;
            PlayerCharacter->bCanChangeWeapon = false;
            PlayerCharacter->bCanDefend = false;
            bIsSpecialAttackActive = true;
        }
    }
}
