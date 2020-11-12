// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"

#include "Arrow.h"
#include "Components/SkeletalMeshComponent.h"

ABow::ABow() : Super()
{
}

void ABow::LightAttack()
{
    Super::LightAttack();
}

void ABow::ShootArrow()
{
    FActorSpawnParameters SpawnParam;
    SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParam.Owner = GetParentActor();
    ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetParentActor());
    const FVector SpawnLocation = BaseCharacter->GetMesh()->GetSocketLocation("RightWeaponShield");
    AArrow* ShotArrow = GetWorld()->SpawnActor<AArrow>(Arrow.Get(), SpawnLocation, GetParentActor()->GetActorForwardVector().Rotation(), SpawnParam);
    if (ShotArrow)
    {
        ShotArrow->SetDamage(GetCurrentDamage());
    }
}

