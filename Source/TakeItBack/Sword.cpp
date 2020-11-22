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
        MeteorShieldDamage = SwordData->MeteorShieldDamage;
        MeteorShieldTimeDilation = SwordData->MeteorShieldTimeDilation;
        MeteorShieldRadius = SwordData->MeteorShieldRadius;
        BlockingSemiAngle = SwordData->BlockingSemiAngle;
        DefenseAnimSpeed = SwordData->DefenseAnimSpeed;
        APlayerCharacter* ParentPlayerCharacter = Cast<APlayerCharacter>(GetParentActor());
        if (ParentPlayerCharacter)
        {
            ParentPlayerCharacter->SetShieldMesh(SwordData->ShieldMesh);
        }
        ShieldAnim = SwordData->ShieldAnim;
        ShieldHitAnim = SwordData->ShieldHitAnim;
        ActiveFOV = SwordData->ActiveFOV;
        SM_CameraOffset = SwordData->SM_CameraOffset;
    }
}

ASword::ASword() : Super()
{
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
        PlayerCharacter->PlayAnimMontage(ShieldAnim, DefenseAnimSpeed);
        bIsBlocking = true;
    }
}

void ASword::StopDefense()
{
    Super::StopDefense();
    bIsBlocking = false;
}

void ASword::ShieldMeteorLaunch()
{
    OnShieldMeteorLaunch();
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
    PlayerCharacter->GetCameraBoom()->SocketOffset -= SM_CameraOffset;
    PlayerCharacter->BaseTurnRate *= MeteorShieldTimeDilation;
    PlayerCharacter->BaseLookUpRate *= MeteorShieldTimeDilation;
    PlayerCharacter->GetCharacterMovement()->GravityScale = PlayerCharacter->GravityScale;
    PlayerCharacter->GetCharacterMovement()->AirControl = PlayerCharacter->AirControl;

    bIsSpecialAttackActive = false;
    bIsLaunched = true;
    PlayerCharacter->bCanAttack = true;
    PlayerCharacter->bCanChangeWeapon = true;
    PlayerCharacter->bCanDefend = true;
    Power = 0;
}

bool ASword::CanTakeDamage(FVector Direction)
{
    Direction.Normalize();
    if (bIsBlocking &&
        FVector::DotProduct(Direction, GetParentActor()->GetActorForwardVector()) >=
        UKismetMathLibrary::DegCos(BlockingSemiAngle))
    {
        if(IsValid(ShieldHitAnim))
            GetParentCharacter()->PlayAnimMontage(ShieldHitAnim);
        return false;
    }
    return true;
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
            OnShieldMeteorStart();
            FVector LaunchVelocity = FVector(0.f);
            LaunchVelocity.Z = UKismetMathLibrary::Sqrt(
                -2 * MeteorShieldJumpHeight * PlayerCharacter->GetCharacterMovement()->GetGravityZ());

            PlayerCharacter->LaunchCharacter(LaunchVelocity, true, true);
            PlayerCharacter->GetCharacterMovement()->AirControl = MeteorShieldAirControl;
            PlayerCharacter->bCanAttack = false;
            PlayerCharacter->bCanChangeWeapon = false;
            PlayerCharacter->bCanDefend = false;
            bIsSpecialAttackActive = true;
            bApexReached = false;
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
                if (!bApexReached)
                {
                    OnShieldMeteorApex();
                    PlayerCharacter->GetCharacterMovement()->Velocity.Z = 0.f;
                    PlayerCharacter->BaseTurnRate /= MeteorShieldTimeDilation;
                    PlayerCharacter->BaseLookUpRate /= MeteorShieldTimeDilation;
                    PlayerCharacter->GetCharacterMovement()->GravityScale = MeteorShieldGravityScale;
                    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), MeteorShieldTimeDilation);
                    PlayerCharacter->GetFollowCamera()->SetFieldOfView(ActiveFOV);
                    PlayerCharacter->GetCameraBoom()->SocketOffset += SM_CameraOffset;
                    bApexReached = true;
                }

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
                PredictParams.ProjectileRadius = PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
                PredictParams.OverrideGravityZ = PlayerCharacter->GetCharacterMovement()->GetGravityZ() *
                   PlayerCharacter->GravityScale / PlayerCharacter->GetCharacterMovement()->GravityScale;

                FPredictProjectilePathResult PredictResult;


                UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, PredictResult);

                DrawDebugSphere(GetWorld(), PredictResult.HitResult.Location,
                                100, 12, FColor::Red,
                                false, 0, 0, 10);

                ElapsedTime += DeltaTime / MeteorShieldTimeDilation;
            }
            else
            {
                ShieldMeteorLaunch();
            }
        }
    }
    if (bIsLaunched && !PlayerCharacter->GetCharacterMovement()->IsFalling())
    {
        OnShieldMeteorImpact();

        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(PlayerCharacter);

        TArray<class AActor*> OutActors;

        UKismetSystemLibrary::SphereOverlapActors(GetWorld(), PlayerCharacter->GetActorLocation(), MeteorShieldRadius,
                                                  ObjectTypes,
                                                  nullptr, ActorsToIgnore, OutActors);

        for (int i = 0; i < OutActors.Num(); i++)
        {
            ABaseCharacter* Enemy = Cast<ABaseCharacter>(OutActors[i]);
            if (Enemy)
            {
                Enemy->MyTakeDamage(MeteorShieldDamage);
            }
        }
        bIsLaunched = false;
    }
}
