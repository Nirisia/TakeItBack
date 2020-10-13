// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"


// Sets default values
AWeapon::AWeapon()
{
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

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


