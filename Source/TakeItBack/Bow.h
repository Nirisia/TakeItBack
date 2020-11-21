// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Weapon.h"
#include "Bow.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API ABow : public AWeapon
{
	GENERATED_BODY()

	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AArrow> Arrow;
	
	public:
	ABow();



	
	virtual void LightAttack() override;
	UFUNCTION(BlueprintCallable)
	void ShootArrow();

	UFUNCTION(BlueprintImplementableEvent)
	void OnShootArrow();

};
