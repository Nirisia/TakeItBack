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
	float LaunchSpeed = 1000.f;

	UPROPERTY()
	float ElapsedTime;

public:
	virtual void LoadDataAssets() override;
	ASword();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UAnimMontage* ShieldAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxAirTime = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MeteorShieldAirControl = 0.05f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MeteorShieldGravityScale = 0.25f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MeteorShieldJumpHeight = 500.f;

	virtual void LightAttack() override;
	virtual void SpecialAttack() override;
	virtual void Defense() override;

	UFUNCTION(BlueprintNativeEvent)
	void ShieldMeteor();

	UFUNCTION(BlueprintNativeEvent)
	void ShieldMeteorTick(float DeltaTime);

	UFUNCTION()
	void ShieldMeteorLaunch();


protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
};
