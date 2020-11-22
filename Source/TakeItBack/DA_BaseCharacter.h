// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "BaseCharacter.h"
#include "Engine/DataAsset.h"
#include "DA_BaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API UDA_BaseCharacter : public UDataAsset
{
	GENERATED_BODY()

	public:

	/* Maximum life of the character */
	UPROPERTY(EditAnywhere, Category="Stats")
	int MaxLife = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Knockback")
	float ReceiveVelocity = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Knockback")
	int KnockbackCoeffDmg = 10.f;

	/* Default Walking Speed of the character */
	UPROPERTY(EditAnywhere, Category="Movement")
	int WalkSpeed = 600.f;

	/* Max rotation speed of the character */
	UPROPERTY(EditAnywhere, Category="Movement")
	FRotator RotationRate = FRotator(0.0f, 540.f, 0.0f);

	/* Max Jump height of the character */
	UPROPERTY(EditAnywhere, Category="Movement")
	float JumpHeight = 200.f;

	/* Speed percentage of the character while in air */
	UPROPERTY(EditAnywhere, Category="Movement")
	float AirControl = 0.2f;

	UPROPERTY(EditAnywhere, Category="Stats")
	EWeaponResistance Resistance;

	/* Percentage of damage applied to the character if resistant*/
	UPROPERTY(EditAnywhere, Category="Stats")
	float ResistanceCoeff = 0.5f;

	/* Gravity scale applied to the character*/
	UPROPERTY(EditAnywhere, Category="Movement")
	float GravityScale = 1.f;

	UPROPERTY(EditAnywhere, Category="Mesh")
	class USkeletalMesh* CharacterMesh;

	UPROPERTY(EditAnywhere, Category="Anim")
	class UAnimMontage* StunAnim;
};
