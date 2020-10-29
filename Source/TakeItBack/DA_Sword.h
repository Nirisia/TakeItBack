// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DA_Weapon.h"
#include "Engine/DataAsset.h"
#include "DA_Sword.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API UDA_Sword : public UDA_Weapon
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, Category="Stats")
	float LaunchSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float MaxAirTime = 4.f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float MeteorShieldAirControl = 0.05f;
	
	UPROPERTY(EditAnywhere, Category="Stats")
	float MeteorShieldGravityScale = 0.25f;
	
	UPROPERTY(EditAnywhere, Category="Stats")
	float MeteorShieldJumpHeight = 500.f;
	
	UPROPERTY(EditAnywhere, Category="Mesh")
	class UStaticMesh* ShieldMesh;

	UPROPERTY(EditAnywhere, Category="Stats")
	class UAnimMontage* ShieldAnim;
	
};
