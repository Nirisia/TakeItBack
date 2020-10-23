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
    if (bCanAttack == true)
    {
        //bCanAttack = false;
        if (Cast<AWeapon>(Weapon->GetChildActor())->AtkCount <= 1)
        {
            GEngine->AddOnScreenDebugMessage(0,1.0f,FColor::Yellow,FString::Printf(TEXT("%i"),Cast<AWeapon>(Weapon->GetChildActor())->AtkCount));
            Cast<AWeapon>(Weapon->GetChildActor())->LightAttack();
        }
        else
        {
            Cast<AWeapon>(Weapon->GetChildActor())->AtkCount = 0;
        }
    }
}

void AEnemyCharacter::ValidateAttack()
{
    bCanAttack = true;
}
