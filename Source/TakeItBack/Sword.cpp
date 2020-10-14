// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"

#include "BaseCharacter.h"
#include "Engine.h"
#include "PlayerCharacter.h"

ASword::ASword() : Super()
{
}

void ASword::LightAttack()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("SwordAttack"));
}

void ASword::SpecialAttack()
{
    ShieldMeteor();
}

void ASword::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter);
    if (PlayerCharacter)
    {
        if (!OwnerCharacter->GetCharacterMovement()->IsFalling())
        {
            if (bIsLaunched)
            {
                Cast<APlayerController>(OwnerCharacter->GetController())->PlayDynamicForceFeedback(
                    1.0f, 0.5f, true, true, true, true);
                bIsLaunched = false;
                PlayerCharacter->bCanAttack = true;
                PlayerCharacter->bCanChangeWeapon = true;
                PlayerCharacter->SphereComponent->SetGenerateOverlapEvents(false);
            }
            else
            {
                bIsShieldMeteorActive = false;
            }
        }
        else if (bIsShieldMeteorActive && PlayerCharacter->GetVelocity().Z > 0)
        {
            UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.5f);
            PlayerCharacter->GetFollowCamera()->SetFieldOfView(80.f);
        }
        else
        {
            UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
            PlayerCharacter->GetFollowCamera()->SetFieldOfView(90.f);
            bIsShieldMeteorActive = false;
            PlayerCharacter->bCanAttack = true;
            PlayerCharacter->bCanChangeWeapon = true;
        }
    }
}

void ASword::ShieldMeteor_Implementation()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
    {
        if (!bIsLaunched && bIsShieldMeteorActive)
        {
            FVector LaunchVelocity = PlayerCharacter->GetFollowCamera()->GetForwardVector();
            if (LaunchVelocity.Z > 0)
            {
                LaunchVelocity.Z = 0;
            }
            LaunchVelocity *= 1000;
            PlayerCharacter->LaunchCharacter(LaunchVelocity, true, true);
            PlayerCharacter->SphereComponent->SetGenerateOverlapEvents(true);
            bIsShieldMeteorActive = false;
            bIsLaunched = true;
        }
        else if (!PlayerCharacter->GetCharacterMovement()->IsFalling())
        {
            PlayerCharacter->LaunchCharacter(FVector(0.f, 0.f, 1000.f), true, true);
            PlayerCharacter->bCanAttack = false;
            PlayerCharacter->bCanChangeWeapon = false;
            bIsShieldMeteorActive = true;
        }
    }
}
