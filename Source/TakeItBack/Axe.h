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

	FDynamicForceFeedbackHandle ForceFeedbackHandle;

	public:
	AAxe();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	class UAnimMontage* RollAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollMaxSpeed = 1800.f;
	
	virtual void SpecialAttack() override;

	virtual void Defense() override;

	virtual void AttackCollision(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;
	
	UFUNCTION(BlueprintNativeEvent)
	void FireStorm();

	UFUNCTION(BlueprintNativeEvent)
	void Roll();

	UFUNCTION(BlueprintCallable)
	void RollStart();
	
	UFUNCTION(BlueprintCallable)
	void RollTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void RollEnd();
	virtual void Tick(float DeltaTime) override;
};
