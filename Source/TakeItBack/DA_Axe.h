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
	UPROPERTY(EditAnywhere, Category="Stats")
	float RollMaxSpeed = 1800.f;
	
	UPROPERTY(EditAnywhere, Category="Stats")
	float FireStormTime = 3.f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float FireStormTurnRate = 3 * 360.f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float FireStormWalkSpeedCoef = 0.25f;
};
