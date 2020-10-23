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
	UPROPERTY(EditAnywhere, Category="Movement")
	int WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	FRotator RotationRate = FRotator(0.0f, 540.f, 0.0f);

	UPROPERTY(EditAnywhere, Category="Movement")
	float JumpHeight = 200.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float JumpTime = 1.f/3.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float AirControl = 0.2f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float GravityScale = 1.f;
};
