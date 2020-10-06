// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AWeapon::LightAttack()
{
}

void AWeapon::BasicAttack()
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

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

