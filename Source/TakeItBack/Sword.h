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
	bool bApexReached = false;

	UPROPERTY()
	float LaunchSpeed = 1000.f;

	UPROPERTY()
	float ElapsedTime;

	UPROPERTY()
	float MeteorShieldDamage = 100;

	UPROPERTY()
	float MeteorShieldTimeDilation = 0.25f;

	UPROPERTY()
	float MeteorShieldRadius = 150.f;

	UPROPERTY()
	bool bIsBlocking;

	UPROPERTY()
	float BlockingSemiAngle;

	UPROPERTY()
	float DefenseAnimSpeed;

public:
	virtual void LoadDataAssets() override;
	ASword();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UAnimMontage* ShieldAnim;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UAnimMontage* ShieldHitAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxAirTime = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MeteorShieldAirControl = 0.05f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MeteorShieldGravityScale = 0.25f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MeteorShieldJumpHeight = 500.f;

	UPROPERTY()
	float ActiveFOV = 80.f;

	UPROPERTY()
	FVector SM_CameraOffset = FVector(0.f, 300.f, 0.f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, Category="Mesh")
	class UStaticMeshComponent* MeteorShieldAimMesh;
	
	virtual void SpecialAttack() override;
	virtual void Defense() override;
	virtual void StopDefense() override;

	UFUNCTION(BlueprintNativeEvent)
	void ShieldMeteor();

	UFUNCTION(BlueprintNativeEvent)
	void ShieldMeteorTick(float DeltaTime);

	UFUNCTION()
	void ShieldMeteorLaunch();

	UFUNCTION(BlueprintImplementableEvent)
    void OnShieldMeteorStart();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnShieldMeteorImpact();

	UFUNCTION(BlueprintImplementableEvent)
	void OnShieldMeteorLaunch();

	UFUNCTION(BlueprintImplementableEvent)
    void OnShieldMeteorApex();

	virtual bool CanTakeDamage(FVector Direction) override;
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
};
