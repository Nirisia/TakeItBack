// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "PlayerCharacter.h"
#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyCharacter.h"

AAxe::AAxe() : Super()
{
}

void AAxe::SpecialAttack()
{
    FireStorm();
    Power = 0;
    
    if(BonusStack < 8)
    {
        BonusStack++;
    }  
}

void AAxe::Defense()
{
    if (!GetParentCharacter()->GetCharacterMovement()->IsFalling())
    {
        Super::Defense();
        Roll(); 
    }
}

void AAxe::AttackCollision(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Enemy"))
    {
        ABaseCharacter* Enemy = Cast<ABaseCharacter>(OtherActor);
        if (Enemy != nullptr)
        {
            Cast<APlayerController>(GetParentCharacter()->GetController())->PlayDynamicForceFeedback(
                    0.2f, 0.3f, false, true, false, true);
            
            LoadPower(Enemy->MyTakeDamage(Damage + BonusStack * DamageBonus *
             Damage));
        }
    }
}

void AAxe::Roll_Implementation()
{
    
}

void AAxe::Tick(float DeltaTime)
{
    if (bIsSpecialAttackActive)
    {
        if (ElapsedTime > 3.0f)
        {
            APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());

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

            bIsSpecialAttackActive = false;
        }
        
        GetParentCharacter()->AddActorWorldRotation(FRotator(0.f, 3 * 360.f * DeltaTime, 0.f ));
        
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
        
        PlayerCharacterMovement->MaxWalkSpeed = 150.f;
        PlayerCharacterMovement->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
        auto PlayerController = Cast<APlayerController>(GetParentCharacter()->GetController());
        ForceFeedbackHandle = PlayerController->PlayDynamicForceFeedback(1.0f, -1.f, true, true, true, true);

        PlayerCharacter->bCanAttack = false;
        PlayerCharacter->bCanSpecialAttack = false;
        PlayerCharacter->bCanChangeWeapon = false;

        SetWeaponCollision(true);
        
        bIsSpecialAttackActive = true;   
    }
}
