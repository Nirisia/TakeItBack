// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::Attack()
{
	bIsAttacking = true;
}

void ABaseCharacter::MyTakeDamage(AWeapon* AttackingWeapon)
{
	bImpact = true;
	CurrentLife -= AttackingWeapon->Damage;
	if(CurrentLife <= 0)
	{
		bIsDead = true;
	}
}

void ABaseCharacter::SetWeaponCollision(bool bGenerateOverlap)
{
	SetWeaponCollisionEvent(bGenerateOverlap);
}

void ABaseCharacter::SetWeaponCollisionEvent_Implementation(bool bGenerateOverlap)
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

