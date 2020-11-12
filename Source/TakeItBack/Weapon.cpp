// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BaseCharacter.h"
#include "DA_Weapon.h"
#include "EnemySpawner.h"
#include "PlayerCharacter.h"
#include "Engine.h"

// Sets default values
AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
    SetRootComponent(MeshComponent);

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxComponent->SetupAttachment(MeshComponent);
    BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    BoxComponent->SetGenerateOverlapEvents(false);
}

void AWeapon::LightAttack()
{
    if (AtkCount >= Attacks.Num())
    {
        AtkCount = 0;
    }
    if (Attacks.Num() != 0)
        GetParentCharacter()->PlayAnimMontage(Attacks[AtkCount].AnimMontage, AtkSpeed);
    AtkCount++;
}

void AWeapon::SpecialAttack()
{
}

void AWeapon::Defense()
{
    auto PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
    if (PlayerCharacter)
    {
        PlayerCharacter->bCanDefend = false;
        PlayerCharacter->bCanSpecialAttack = false;
        PlayerCharacter->bCanChangeWeapon = false;
    }
    GetParentCharacter()->bCanAttack = false;

    AtkCount = 0;
}

void AWeapon::StopDefense()
{
    const auto PlayerCharacter = Cast<APlayerCharacter>(GetParentCharacter());
    if (PlayerCharacter)
    {
        PlayerCharacter->bCanDefend = true;
        PlayerCharacter->bCanSpecialAttack = true;
        PlayerCharacter->bCanChangeWeapon = true;
    }
    PlayerCharacter->bCanAttack = true;
    PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = PlayerCharacter->WalkSpeed;
    PlayerCharacter->GetCharacterMovement()->RotationRate = PlayerCharacter->RotationRate;
}

void AWeapon::LoadPower(int InflictedDamage)
{
    if (!bIsSpecialAttackActive)
    {
        Power += InflictedDamage * WinPower;

        if (Power > MaxPower)
        {
            Power = MaxPower;
        }
    }
}

void AWeapon::SetWeaponCollision(bool bGenerateOverlap)
{
    BoxComponent->SetGenerateOverlapEvents(bGenerateOverlap);
}

int AWeapon::GetCurrentDamage()
{
    if (AtkCount == 0) return 0;
    return Attacks[AtkCount - 1].Damage;
}

void AWeapon::LoadDataAssets()
{
    if (WeaponData)
    {
        MaxPower = WeaponData->MaxPower;
        WalkSpeedCoef = WeaponData->WalkSpeedCoef;
        GravityScaleCoef = WeaponData->GravityScaleCoef;
        AtkSpeed = WeaponData->AtkSpeed;
        WinPower = WeaponData->WinPower;
        MeshComponent->SetStaticMesh(WeaponData->Mesh);
        Attacks = WeaponData->Attacks;
        WeaponType = WeaponData->WeaponType;
    }
}

void AWeapon::AttackCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
    if (OtherActor == GetParentActor()) return;
    if (OtherActor->ActorHasTag("Enemy") || OtherActor->ActorHasTag("Player"))
    {
        ABaseCharacter* Enemy = Cast<ABaseCharacter>(OtherActor);
        if (IsValid(Enemy))
        {
            LoadPower(Enemy->MyTakeDamage(GetCurrentDamage(), WeaponType) * WinPower);
        }
    }
    else if (OtherActor->ActorHasTag("Spawn"))
    {
        AEnemySpawner* Spawn = Cast<AEnemySpawner>(OtherActor);
        if (IsValid(Spawn))
        {
            Spawn->MyTakeDamage(GetCurrentDamage());
        }
    }
}

void AWeapon::ResetCombo()
{
    AtkCount = 0;
}

ABaseCharacter* AWeapon::GetParentCharacter()
{
    return Cast<ABaseCharacter>(GetParentActor());
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
    Super::BeginPlay();
    LoadDataAssets();
    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::AttackCollision);
}

void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
