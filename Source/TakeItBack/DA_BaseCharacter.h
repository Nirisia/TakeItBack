// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	/* Gravity scale applied to the character*/
	UPROPERTY(EditAnywhere, Category="Movement")
	float GravityScale = 1.f;

	UPROPERTY(EditAnywhere, Category="Mesh")
	class USkeletalMesh* CharacterMesh;
};