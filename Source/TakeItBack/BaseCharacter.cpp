// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "DA_BaseCharacter.h"
#include "Engine.h"
#include "Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void ABaseCharacter::LoadDataAssets()
{
	if (CharacterData)
	{
		MaxLife = CharacterData->MaxLife;
		JumpHeight = CharacterData->JumpHeight;
		AirControl = CharacterData->AirControl;
		WalkSpeed = CharacterData->WalkSpeed;
		RotationRate = CharacterData->RotationRate;
		GravityScale = CharacterData->GravityScale;
		GetMesh()->SetSkeletalMesh(CharacterData->CharacterMesh);
		Resistance = CharacterData->Resistance;
		ResistanceCoeff = CharacterData->ResistanceCoeff;
	}
}
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseCharacter::Die()
{
	CurrentLife = 0;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsDead = true;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	GetCharacterMovement()->RotationRate = FRotator(0);
	GetCharacterMovement()->SetJumpAllowed(false);
	bCanAttack = false;
}

void ABaseCharacter::Revive()
{
	CurrentLife = MaxLife;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bIsDead = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->RotationRate = RotationRate;
	GetCharacterMovement()->SetJumpAllowed(true);
	bCanAttack = true;
}

void ABaseCharacter::Attack()
{
	
}

int ABaseCharacter::MyTakeDamage(int Damage, EWeaponResistance WeaponType)
{
	if (CurrentLife > 0)
	{
		ResetCombo();
		int InflictedDamage;
		if (WeaponType == Resistance)
		{
			InflictedDamage = Damage * ResistanceCoeff;
		}
		
 	// TODO: Variables
 		// else if (WeaponType != EWeaponResistance::None)
 		// {
 		// 	InflictedDamage = Damage * WeaknessCoeff;
 		// }
		
		else
		{
			InflictedDamage = Damage;
		}

		CurrentLife -= InflictedDamage;
		LaunchCharacter(-GetActorForwardVector()*ReceiveVelocity*(InflictedDamage/KnockbackCoeffDmg), false, false);
		
		if(CurrentLife <= 0)
		{
			Die();
			return InflictedDamage + CurrentLife;
		}
		return InflictedDamage;
	}
	return 0;	

}

void ABaseCharacter::SetWeaponCollision(bool bGenerateOverlap)
{
	if (GetCurrentWeapon())
	{
		GetCurrentWeapon()->SetWeaponCollision(bGenerateOverlap);
	}
}

AWeapon* ABaseCharacter::GetCurrentWeapon()
{
	return nullptr;
}

void ABaseCharacter::ValidateAttack()
{
	if (bIsDead) return;
	bCanAttack = true;
}

void ABaseCharacter::ResetCombo()
{
	if (bIsDead) return;
	bCanAttack = true;
	GetCharacterMovement()->RotationRate = RotationRate;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SetWeaponCollision(false);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	LoadDataAssets();
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->JumpZVelocity = UKismetMathLibrary::Sqrt(-2 * JumpHeight * Movement->GetGravityZ() * GravityScale);
	Movement->AirControl = AirControl;
	Movement->MaxWalkSpeed = WalkSpeed;
	Movement->RotationRate = RotationRate;
	Movement->GravityScale = GravityScale;

	CurrentLife = MaxLife;
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

