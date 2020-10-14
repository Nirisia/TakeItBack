// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GameFramework/PlayerController.h"

#include "Axe.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API AAxe : public AWeapon
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsFireStormActive;

	UPROPERTY()
	float ElapsedTime = 0.f;

	FDynamicForceFeedbackHandle ForceFeedbackHandle;

	AAxe();
	public:
	virtual void LightAttack() override;
	virtual void SpecialAttack() override;

	UFUNCTION(BlueprintNativeEvent)
	void FireStorm();

	virtual void Tick(float DeltaTime) override;
};
