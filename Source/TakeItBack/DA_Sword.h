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

	UPROPERTY(EditAnywhere, Category="Stats")
	float MaxAirTime = 4.f;

	UPROPERTY(EditAnywhere, Category="Mesh")
	class UStaticMesh* ShieldMesh;

	UPROPERTY(EditAnywhere, Category="Stats")
	class UAnimMontage* ShieldAnim;
	
};
