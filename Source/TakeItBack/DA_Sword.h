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
	/* Speed at which the character is launched with ShieldMeteor */
	UPROPERTY(EditAnywhere, Category="Stats")
	float LaunchSpeed = 1000.f;

	/* Maximum time the character spend in slow motion while at apex of the jump */
	UPROPERTY(EditAnywhere, Category="Stats")
	float MaxAirTime = 4.f;

	/* Percentage of the default character speed while in air for ShieldMeteor  */
	UPROPERTY(EditAnywhere, Category="Stats")
	float MeteorShieldAirControl = 0.05f;

	/* Gravity scale applied to the player while in slowmo*/
	UPROPERTY(EditAnywhere, Category="Stats")
	float MeteorShieldGravityScale = 0.25f;

	/* Maximum jump height of the player with ShieldMeteor */
	UPROPERTY(EditAnywhere, Category="Stats")
	float MeteorShieldJumpHeight = 500.f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float MeteorShieldDamage = 100;

	UPROPERTY(EditAnywhere, Category="Stats")
	float MeteorShieldTimeDilation = 0.25f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float MeteorShieldRadius = 150.f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float BlockingSemiAngle = 45.f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float DefenseAnimSpeed = 1.0f;
	
	UPROPERTY(EditAnywhere, Category="Mesh")
	class UStaticMesh* ShieldMesh;

	UPROPERTY(EditAnywhere, Category="Stats")
	class UAnimMontage* ShieldAnim;

	UPROPERTY(EditAnywhere, Category="Stats")
	class UAnimMontage* ShieldHitAnim;

	/* FOV used during ShieldMeteor */
	UPROPERTY(EditAnywhere, Category="Camera")
	float ActiveFOV = 80.f;

	/* Horizontal offset of the camera during ShieldCamera. Positive is on right, negative on left */
	UPROPERTY(EditAnywhere, Category="Camera")
	FVector SM_CameraOffset = FVector(0.f, 300.f, 0.f);
};
