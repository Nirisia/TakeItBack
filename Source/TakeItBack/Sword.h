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
	bool bIsShieldMeteorActive;

	UPROPERTY()
	bool bIsLaunched;
	
	ASword();
	public:
	virtual void LightAttack() override;
	virtual void SpecialAttack() override;
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintNativeEvent)
	void ShieldMeteor();

public:
	virtual void Tick(float DeltaTime) override;
};
