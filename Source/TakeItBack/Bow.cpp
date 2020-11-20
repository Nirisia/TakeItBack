// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"

#include "Arrow.h"
#include "EnemyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"

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
    AArrow* ShotArrow = GetWorld()->SpawnActor<AArrow>(Arrow.Get(), SpawnLocation,
                                                       GetParentActor()->GetActorForwardVector().Rotation(),
                                                       SpawnParam);
    if (ShotArrow)
    {
        ShotArrow->SetDamage(GetCurrentDamage());
        AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(BaseCharacter);
        if (EnemyCharacter)
        {
            FVector NewVelocity;

            // Yeah that's bad but it's the easiest way now to get player character from the bow
            // TODO: Replace this by a proper implementation
            FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();

            UGameplayStatics::SuggestProjectileVelocity(GetWorld(), NewVelocity, ShotArrow->GetActorLocation(),
                                                        PlayerLocation, ShotArrow->GetInitialVelocity(), false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace);
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue,
                                             FString::Printf(
                                                 TEXT("x:%f, y:%f, z:%f"), NewVelocity.X, NewVelocity.Y,
                                                 NewVelocity.Z));
            ShotArrow->SetVelocity(NewVelocity);
        }
    }
}
