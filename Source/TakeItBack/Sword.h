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

	UPROPERTY()
	bool bIsLaunched;

	UPROPERTY()
	float ElapsedTime;
	
	public:
	ASword();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Animations")
	class UAnimMontage* ShieldAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats")
	float MaxAirTime = 4;

	virtual void LightAttack() override;
	virtual void SpecialAttack() override;
	virtual void Defense() override;

	UFUNCTION(BlueprintNativeEvent)
	void ShieldMeteor();

	UFUNCTION(BlueprintNativeEvent)
	void ShieldMeteorTick(float DeltaTime);

	UFUNCTION()
	void ShieldMeteorLaunch();
	

	virtual void Tick(float DeltaTime) override;
};
