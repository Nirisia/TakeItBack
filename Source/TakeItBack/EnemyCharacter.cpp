// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemySpawner.h"
#include "Weapon.h"
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

void AEnemyCharacter::ResetCombo()
{
    Super::ResetCombo();
    Cast<AWeapon>(Weapon->GetChildActor())->ResetCombo();
}

void AEnemyCharacter::LoadDataAssets()
{
    Super::LoadDataAssets();
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
            CurrentWeapon->AtkCount = 0;
            Timer = GetWorld()->GetTimeSeconds();
        }
    }
}

void AEnemyCharacter::Die()
{
    Super::Die();
    if (Spawner)
    {
        Spawner->OnEnemyDie(this);
    }
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    Super::LoadDataAssets();
}
