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
	UPROPERTY(EditAnywhere, Category="Stats")
	int MaxPower = 300;

	UPROPERTY(EditAnywhere, Category="Stats")
	int Damage = 9;

	UPROPERTY(EditAnywhere, Category="Stats")
	float AtkSpeed = 1.f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float AtkSpeedBonus = 1.f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float DamageBonus = 1.f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float WinPower = 1.f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float LosePower = 1.f;

	UPROPERTY(EditAnywhere, Category="Mesh")
	class UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, Category="Mesh")
	FBox BoxCollision;

	UPROPERTY(EditAnywhere, Category="Animations")
	TArray<class UAnimMontage*> AttacksAnim;
};
