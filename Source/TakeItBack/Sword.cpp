// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "BaseCharacter.h"
#include "Engine.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

ASword::ASword() : Super() {}

void ASword::LightAttack()
{
    if (AtkCount >= AttacksAnim.Num())
    {
        AtkCount = 0;
    }

    GetParentCharacter()->PlayAnimMontage(AttacksAnim[AtkCount], AtkSpeed + BonusStack * SpeedBonus * AtkSpeed);
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
        PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 0.f;
        PlayerCharacter->GetCharacterMovement()->RotationRate = (FRotator(0));
        PlayerCharacter->PlayAnimMontage(ShieldAnim);
    }
}

void ASword::ShieldMeteorLaunch()
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());

    FVector LaunchVelocity = PlayerCharacter->GetFollowCamera()->GetForwardVector();
    if (LaunchVelocity.Z > 0)
    {
        LaunchVelocity.Z = 0;
    }
    LaunchVelocity *= 1000;
    PlayerCharacter->LaunchCharacter(LaunchVelocity, true, true);
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
    PlayerCharacter->GetFollowCamera()->SetFieldOfView(90.f);
    PlayerCharacter->GetCharacterMovement()->GravityScale = 1.f;
    PlayerCharacter->GetCharacterMovement()->AirControl = 0.05f;
    
    PlayerCharacter->SphereComponent->SetGenerateOverlapEvents(true);
    bIsSpecialAttackActive = false;
    bIsLaunched = true;
    PlayerCharacter->bCanAttack = true;
    PlayerCharacter->bCanChangeWeapon = true;
    PlayerCharacter->bCanDefend = true;
    Power = 0;

    if (BonusStack < 8)
    {
        BonusStack++;
    }
}

void ASword::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
    if (PlayerCharacter)
    {
        ShieldMeteorTick(DeltaTime);
    }
}

void ASword::ShieldMeteor_Implementation()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter()))
    {
        if (bIsSpecialAttackActive)
        {
            ShieldMeteorLaunch();
        }
        else if (!PlayerCharacter->GetCharacterMovement()->IsFalling())
        {
            Cast<APlayerController>(GetParentCharacter()->GetController())->PlayDynamicForceFeedback(
                0.05f, 0.3f, false, true, false, true);
            PlayerCharacter->LaunchCharacter(FVector(0.f, 0.f, 1000.f), true, true);
            PlayerCharacter->GetCharacterMovement()->AirControl = 0.01f;
            PlayerCharacter->bCanAttack = false;
            PlayerCharacter->bCanChangeWeapon = false;
            PlayerCharacter->bCanDefend = false;
            bIsSpecialAttackActive = true;
            ElapsedTime = 0.f;
        }
    }
}

void ASword::ShieldMeteorTick_Implementation(float DeltaTime)
{
    APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
    if (bIsSpecialAttackActive)
    {
        if (PlayerCharacter->GetCharacterMovement()->Velocity.Z <= 0)
        {
            if (ElapsedTime < MaxAirTime)
            {
                PlayerCharacter->GetCharacterMovement()->GravityScale = 0.25f / 10;
                UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25f);
                PlayerCharacter->GetFollowCamera()->SetFieldOfView(80.f);
                ElapsedTime += DeltaTime / 0.25f;
            }
            else
            {
                ShieldMeteorLaunch();
            }
        }
    }
    if (bIsLaunched && !PlayerCharacter->GetCharacterMovement()->IsFalling())
    {
        Cast<APlayerController>(GetParentCharacter()->GetController())->PlayDynamicForceFeedback(
            1.0f, 0.5f, true, true, true, true);
        bIsLaunched = false;
        PlayerCharacter->SphereComponent->SetGenerateOverlapEvents(false);
    }
}
