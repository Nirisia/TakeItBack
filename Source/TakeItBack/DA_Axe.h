// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DA_Weapon.h"
#include "Engine/DataAsset.h"
#include "DA_Axe.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API UDA_Axe : public UDA_Weapon
{
	GENERATED_BODY()

	public:
	/* Max Speed reached during roll */
	UPROPERTY(EditAnywhere, Category="Stats")
	float RollMaxSpeed = 1800.f;

	/* Total time of FireStorm */
	UPROPERTY(EditAnywhere, Category="Stats")
	float FireStormTime = 3.f;

	/* Angular Speed of the player during FireStorm. In deg/sec */
	UPROPERTY(EditAnywhere, Category="Stats")
	float FireStormTurnRate = 3 * 360.f;

	/* Speed coefficient applied to speed during FireStorm*/
	UPROPERTY(EditAnywhere, Category="Stats")
	float FireStormWalkSpeedCoef = 0.25f;

	/* FOV used during FireStorm */
	UPROPERTY(EditAnywhere, Category="Camera")
	float ActiveFOV = 100.f;
};
