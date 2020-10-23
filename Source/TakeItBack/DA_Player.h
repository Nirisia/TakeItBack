// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DA_BaseCharacter.h"
#include "Engine/DataAsset.h"

#include "DA_Player.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API UDA_Player : public UDA_BaseCharacter
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, Category="Stats")
	int MaxLife = 200;

	UPROPERTY(EditAnywhere, Category="Stats")
	float ChangeWeaponSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category="Stats")
	int StackLimit = 8;

	UPROPERTY(EditAnywhere, Category="Mesh")
	class USkeletalMesh* PlayerMesh;
};
