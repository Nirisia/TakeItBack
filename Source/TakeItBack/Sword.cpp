// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "BaseCharacter.h"
#include "Engine.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

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
        if (bIsSpecialAttackActive && ElapsedTime > 0.f)
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
                PlayerCharacter->GetCharacterMovement()->GravityScale = 0.25f;
                UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25f);
                PlayerCharacter->GetFollowCamera()->SetFieldOfView(80.f);

                FPredictProjectilePathParams PredictParams;

                PredictParams.StartLocation = PlayerCharacter->GetActorLocation();

                FVector LaunchVelocity = PlayerCharacter->GetFollowCamera()->GetForwardVector();
                if (LaunchVelocity.Z > 0)
                {
                    LaunchVelocity.Z = 0;
                }
                LaunchVelocity *= 1000;

                TArray<AActor*> ActorsToIgnore;
                ActorsToIgnore.Add(PlayerCharacter);

                PredictParams.LaunchVelocity = LaunchVelocity;
                PredictParams.TraceChannel = ECC_WorldDynamic;
                PredictParams.TraceChannel = ECC_WorldStatic;
                PredictParams.bTraceWithChannel = true;
                PredictParams.bTraceWithCollision = true;
                PredictParams.ActorsToIgnore = ActorsToIgnore;
                PredictParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;

                FPredictProjectilePathResult PredictResult;


                UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, PredictResult);

                DrawDebugSphere(GetWorld(), PredictResult.HitResult.Location,
                    100, 12, FColor::Red,
                    false, 0, 0, 10);


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

        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(PlayerCharacter);

        TArray<class AActor*> OutActors;

        UKismetSystemLibrary::SphereOverlapActors(GetWorld(), PlayerCharacter->GetActorLocation(), 150, ObjectTypes,
                                                  nullptr, ActorsToIgnore, OutActors);

        for (int i = 0; i < OutActors.Num(); i++)
        {
            ABaseCharacter* Enemy = Cast<ABaseCharacter>(OutActors[i]);
            if (Enemy)
            {
                Enemy->MyTakeDamage(100);
                if (Enemy->bIsDead)
                {
                    USkeletalMeshComponent* EnemyMesh = Enemy->GetMesh();
                    FVector Direction = Enemy->GetActorLocation() - PlayerCharacter->GetActorLocation();
                    Direction.Z *= -1;
                    Direction.Normalize();
                    EnemyMesh->AddImpulseToAllBodiesBelow(Direction * 1000, EnemyMesh->GetBoneName(0), true);
                }
            }
        }

        bIsLaunched = false;
    }
}
