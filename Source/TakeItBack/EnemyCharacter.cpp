// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Engine/Engine.h"

AEnemyCharacter::AEnemyCharacter()
{
    MaxLife = 100;
    CurrentLife = 100;

    Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
    Weapon->SetupAttachment(Cast<USceneComponent>(GetMesh()), "RightWeaponShield");
    Weapon->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));
}

void AEnemyCharacter::SetWeaponCollision(bool bGenerateOverlap)
{
    Cast<AWeapon>(Weapon->GetChildActor())->SetWeaponCollision(bGenerateOverlap);
}

void AEnemyCharacter::Attack()
{
    float TimeElapsed = GetWorld()->GetTimeSeconds() - Timer;
    
    if (bCanAttack == true && TimeElapsed >= AttackDelay)
    {
        bCanAttack = false;
        AWeapon* CurrentWeapon =  Cast<AWeapon>(Weapon->GetChildActor());
        CurrentWeapon->LightAttack();
        if(CurrentWeapon->AtkCount >= CurrentWeapon->AttacksAnim.Num())
        {
            Timer = GetWorld()->GetTimeSeconds();
        }
    }
}

void AEnemyCharacter::ValidateAttack()
{
    bCanAttack = true;
}
