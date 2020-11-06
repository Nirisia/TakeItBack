// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API UDA_Weapon : public UDataAsset
{
	GENERATED_BODY()

	public:
	/* Maximum power of the weapon */
	UPROPERTY(EditAnywhere, Category="Stats")
	int MaxPower = 300;

	/* Base Damage of the weapon */
	UPROPERTY(EditAnywhere, Category="Stats")
	int Damage = 9;

	/* Attack speed of the weapon. Percentage of default animation PlayRate */
	UPROPERTY(EditAnywhere, Category="Stats")
	float AtkSpeed = 1.f;

	/* !DEPRECATED! Bonus Speed gained with special attack stack. Percentage of Default AtkSpeed. */
	UPROPERTY(EditAnywhere, Category="Stats")
	float AtkSpeedBonus = 1.f;

	/* !DEPRECATED! Bonus Damage gained with special attack stack. Percentage of Default Damage. */
	UPROPERTY(EditAnywhere, Category="Stats")
	float DamageBonus = 1.f;

	/* Ratio of power won by damage inflicted */
	UPROPERTY(EditAnywhere, Category="Stats")
	float WinPower = 1.f;

	UPROPERTY(EditAnywhere, Category="Mesh")
	class UStaticMesh* Mesh;

	/* Total combos are defined by the number of animations in this array */
	UPROPERTY(EditAnywhere, Category="Animations")
	TArray<class UAnimMontage*> AttacksAnim;
};
