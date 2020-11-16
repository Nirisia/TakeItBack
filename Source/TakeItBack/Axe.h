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
	float ElapsedTime = 0.f;

	public:
	AAxe();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	class UAnimMontage* RollAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollMaxSpeed = 1800.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FireStormTime = 3.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FireStormTurnRate = 3 * 360.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FireStormWalkSpeedCoef = 0.25f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FireStormDamage = 10.f;

	UPROPERTY()
	float ActiveFOV = 100.f;
	
	virtual void SpecialAttack() override;

	virtual void Defense() override;	
	UFUNCTION(BlueprintNativeEvent)
	void FireStorm();


	virtual void LoadPower(int InflictedDamage) override;
	virtual int GetCurrentDamage() override;
	UFUNCTION(BlueprintNativeEvent)
	void Roll();

	UFUNCTION(BlueprintCallable)
	void RollStart();
	
	UFUNCTION(BlueprintCallable)
	void RollTick(float DeltaTime);

	virtual void LoadDataAssets() override;
	UFUNCTION(BlueprintCallable)
	void RollEnd();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnFireStormStart();
	
	UFUNCTION(BlueprintImplementableEvent)
    void OnFireStormEnd();
};
