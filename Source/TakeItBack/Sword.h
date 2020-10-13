// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Sword.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API ASword : public AWeapon
{
	GENERATED_BODY()

	ASword();
	public:
	virtual void LightAttack() override;

	UFUNCTION(BlueprintCallable)
	virtual void SpecialAttack() override;

	UFUNCTION(BlueprintNativeEvent)
	void ShieldMeteor();
};
