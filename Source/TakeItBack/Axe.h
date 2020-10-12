// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Axe.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API UAxe : public UWeapon
{
	GENERATED_BODY()

	UAxe();
	public:
	virtual void LightAttack() override;
	virtual void BasicAttack() override;
	virtual void SpecialAttack() override;
};
