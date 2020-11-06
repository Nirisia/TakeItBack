// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "DA_BaseCharacter.h"
#include "Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void ABaseCharacter::LoadDataAssets()
{
	if (CharacterData)
	{
		JumpHeight = CharacterData->JumpHeight;
		AirControl = CharacterData->AirControl;
		WalkSpeed = CharacterData->WalkSpeed;
		RotationRate = CharacterData->RotationRate;
		GravityScale = CharacterData->GravityScale;
		GetMesh()->SetSkeletalMesh(CharacterData->CharacterMesh); 
	}
}
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterData = CreateDefaultSubobject<UDA_BaseCharacter>("CharacterData");
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
			GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetMesh()->SetAllBodiesSimulatePhysics(true);
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
	LoadDataAssets();
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->JumpZVelocity = UKismetMathLibrary::Sqrt(-2 * JumpHeight * Movement->GetGravityZ() * GravityScale);
	Movement->AirControl = AirControl;
	Movement->MaxWalkSpeed = WalkSpeed;
	Movement->RotationRate = RotationRate;
	Movement->GravityScale = GravityScale;
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

