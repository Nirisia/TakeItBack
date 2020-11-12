// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BaseCharacter.h"
#include "Engine/DataAsset.h"
#include "DA_Weapon.generated.h"

USTRUCT(BlueprintType)
struct FWeaponAttack
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly)
	float Damage;
};

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

	/* Coefficient of default walkspeed used when this weapon is equipped */
	UPROPERTY(EditAnywhere, Category="Stats")
	float WalkSpeedCoef = 1.0f;

	/* Coefficient of default gravity scale used when this weapon is equipped */
	UPROPERTY(EditAnywhere,Category="Stats")
	float GravityScaleCoef = 1.0f;

	/* Attack speed of the weapon. Percentage of default animation PlayRate */
	UPROPERTY(EditAnywhere, Category="Stats")
	float AtkSpeed = 1.f;

	/* Ratio of power won by damage inflicted */
	UPROPERTY(EditAnywhere, Category="Stats")
	float WinPower = 1.f;

	UPROPERTY(EditAnywhere, Category="Mesh")
	class UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, Category="Stats")
	EWeaponResistance WeaponType;

	/* Total combos are defined by the number of animations in this array */
	UPROPERTY(EditAnywhere, Category="Animations")
	TArray<FWeaponAttack> Attacks;
};
