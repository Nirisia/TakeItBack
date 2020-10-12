// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"


// Sets default values
UWeapon::UWeapon()
{
}

void UWeapon::LightAttack()
{
}

void UWeapon::BasicAttack()
{
}

void UWeapon::SpecialAttack()
{
}

void UWeapon::LoadPower(int InflictedDamage)
{
	Power += InflictedDamage * WinPower;

	if(Power > MaxPower)
	{
		Power = MaxPower;
	}
}

void UWeapon::UnloadPower(int DamageTaken)
{
	Power -= DamageTaken * LosePower;

	if(Power < 0)
	{
		Power = 0;
	}
}

// Called when the game starts or when spawned
void UWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void UWeapon::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


