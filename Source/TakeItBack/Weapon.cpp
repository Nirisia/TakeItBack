// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BaseCharacter.h"


// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(MeshComponent);
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxComponent->SetGenerateOverlapEvents(false);

}

void AWeapon::LightAttack()
{
}

void AWeapon::SpecialAttack()
{
}

void AWeapon::LoadPower(int InflictedDamage)
{
	Power += InflictedDamage * WinPower;

	if(Power > MaxPower)
	{
		Power = MaxPower;
	}
}

void AWeapon::UnloadPower(int DamageTaken)
{
	Power -= DamageTaken * LosePower;

	if(Power < 0)
	{
		Power = 0;
	}
}

void AWeapon::SetWeaponCollision(bool bGenerateOverlap)
{
	BoxComponent->SetGenerateOverlapEvents(bGenerateOverlap);
}

void AWeapon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ABaseCharacter>(GetParentActor());

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlap);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


