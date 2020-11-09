// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "BaseCharacter.h"
#include "DA_Sword.h"
#include "Engine.h"
#include "MainPlayerController.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void ASword::LoadDataAssets()
{
    Super::LoadDataAssets();
    UDA_Sword* SwordData = Cast<UDA_Sword>(WeaponData);
    if (SwordData)
    {
        LaunchSpeed = SwordData->LaunchSpeed;
        MaxAirTime = SwordData->MaxAirTime;
        MeteorShieldAirControl = SwordData->MeteorShieldAirControl;
        MeteorShieldGravityScale = SwordData->MeteorShieldGravityScale;
        MeteorShieldJumpHeight = SwordData->MeteorShieldJumpHeight;
        APlayerCharacter* ParentPlayerCharacter = Cast<APlayerCharacter>(GetParentActor());
        if (ParentPlayerCharacter)
        {
            ParentPlayerCharacter->SetShieldMesh(SwordData->ShieldMesh);
        }
        ShieldAnim = SwordData->ShieldAnim;
        ActiveFOV = SwordData->ActiveFOV;
        SM_RightOffset = SwordData->SM_RightOffset;
    }
}

ASword::ASword() : Super() {}

void ASword::LightAttack()
{
    if (AtkCount >= AttacksAnim.Num())
    {
        AtkCount = 0;
    }

    GetParentCharacter()->PlayAnimMontage(AttacksAnim[AtkCount], AtkSpeed + BonusStack * AtkSpeedBonus * AtkSpeed);
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
    auto PlayerController = Cast<AMainPlayerController>(PlayerCharacter->GetController());

    FVector LaunchVelocity = PlayerCharacter->GetFollowCamera()->GetForwardVector();
    if (LaunchVelocity.Z > 0)
    {
        LaunchVelocity.Z = 0;
    }
    LaunchVelocity *= LaunchSpeed;
    PlayerCharacter->LaunchCharacter(LaunchVelocity, true, true);
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
    PlayerCharacter->GetFollowCamera()->SetFieldOfView(PlayerController->BaseFOV);
    PlayerCharacter->GetCharacterMovement()->GravityScale = PlayerCharacter->GravityScale;
    PlayerCharacter->GetCharacterMovement()->AirControl = PlayerCharacter->AirControl;

    bIsSpecialAttackActive = false;
    bIsLaunched = true;
    PlayerCharacter->bCanAttack = true;
    PlayerCharacter->bCanChangeWeapon = true;
    PlayerCharacter->bCanDefend = true;
    Power = 0;
}

void ASword::BeginPlay()
{
    Super::BeginPlay();

    LoadDataAssets();
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

            FVector LaunchVelocity = 	FVector(0.f);
            LaunchVelocity.Z = UKismetMathLibrary::Sqrt(-2 * MeteorShieldJumpHeight * PlayerCharacter->GetCharacterMovement()->GetGravityZ());

            PlayerCharacter->LaunchCharacter(LaunchVelocity, true, true);
            PlayerCharacter->GetCharacterMovement()->AirControl = MeteorShieldAirControl;
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
                PlayerCharacter->GetCharacterMovement()->GravityScale = MeteorShieldGravityScale;
                UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.25f);
                PlayerCharacter->GetFollowCamera()->SetFieldOfView(ActiveFOV);
                PlayerCharacter->GetCameraBoom()->SocketOffset.Y = SM_RightOffset;

                FPredictProjectilePathParams PredictParams;

                PredictParams.StartLocation = PlayerCharacter->GetActorLocation();

                FVector LaunchVelocity = PlayerCharacter->GetFollowCamera()->GetForwardVector();
                if (LaunchVelocity.Z > 0)
                {
                    LaunchVelocity.Z = 0;
                }
                LaunchVelocity *= LaunchSpeed;

                TArray<AActor*> ActorsToIgnore;
                ActorsToIgnore.Add(PlayerCharacter);

                PredictParams.LaunchVelocity = LaunchVelocity;
                PredictParams.TraceChannel = ECC_Pawn;
                PredictParams.bTraceWithChannel = true;
                PredictParams.bTraceWithCollision = true;
                PredictParams.ActorsToIgnore = ActorsToIgnore;
                PredictParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
                PredictParams.ProjectileRadius = 10.f;

                FPredictProjectilePathResult PredictResult;


                UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, PredictResult);

                DrawDebugSphere(GetWorld(), PredictResult.HitResult.Location,
                    100, 12, FColor::Red,
                    false, 0, 0, 10);


                ElapsedTime += DeltaTime / 0.25f;
            }
            else
            {
                PlayerCharacter->GetCameraBoom()->SocketOffset.Y = 0.f;
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
