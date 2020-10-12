// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Sword.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API USword : public UWeapon
{
	GENERATED_BODY()

	USword();
	public:
	virtual void LightAttack() override;
	virtual void BasicAttack() override;
	virtual void SpecialAttack() override;
};
