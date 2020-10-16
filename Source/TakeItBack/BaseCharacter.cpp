// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Engine.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::Attack()
{
	
}

int ABaseCharacter::MyTakeDamage(int Damage)
{
	if (CurrentLife > 0)
	{
		bImpact = true;
		CurrentLife -= Damage;
		if(CurrentLife <= 0)
		{
			bIsDead = true;
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//Die()
			return Damage + CurrentLife;
		}
		return Damage;
	}
	else
	{
		return 0;	
	}

}

void ABaseCharacter::SetWeaponCollision(bool bGenerateOverlap)
{
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

