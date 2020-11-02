// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BaseCharacter.h"
#include "DA_Weapon.h"
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
    if (AtkCount >= AttacksAnim.Num())
    {
        AtkCount = 0;
    }
    if (AttacksAnim.Num() != 0)
        GetParentCharacter()->PlayAnimMontage(AttacksAnim[AtkCount], AtkSpeed);
    AtkCount++;
}

void AWeapon::SpecialAttack() {}

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

void AWeapon::UnloadPower(int DamageTaken)
{
    Power -= DamageTaken * LosePower;

    if (Power < 0)
    {
        Power = 0;
    }
}

void AWeapon::SetWeaponCollision(bool bGenerateOverlap)
{
    BoxComponent->SetGenerateOverlapEvents(bGenerateOverlap);
}

void AWeapon::LoadDataAssets()
{
    if (WeaponData)
    {
        MaxPower = WeaponData->MaxPower;
        Damage = WeaponData->Damage;
        AtkSpeed = WeaponData->AtkSpeed;
        AtkSpeedBonus = WeaponData->AtkSpeedBonus;
        DamageBonus = WeaponData->DamageBonus;
        WinPower = WeaponData->WinPower;
        LosePower = WeaponData->LosePower;
        MeshComponent->SetStaticMesh(WeaponData->Mesh);
        BoxComponent->SetBoxExtent(WeaponData->BoxCollision.GetExtent());
        AttacksAnim = WeaponData->AttacksAnim;
    }
}

void AWeapon::AttackCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag("Enemy"))
    {
        ABaseCharacter* Enemy = Cast<ABaseCharacter>(OtherActor);
        if (Enemy != nullptr)
        {
            Cast<APlayerController>(GetParentCharacter()->GetController())->PlayDynamicForceFeedback(
                0.2f, 0.3f, false, true, false, true);

            LoadPower(Enemy->MyTakeDamage(Damage));
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
