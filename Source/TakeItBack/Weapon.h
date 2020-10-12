// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Weapon.generated.h"



UCLASS(meta=(BlueprintSpawnableComponent), Blueprintable)
class TAKEITBACK_API UWeapon : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	int MaxPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	int Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float SpeedBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float DamageBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float WinPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float LosePower;
	
	UWeapon();
	
	UFUNCTION(BlueprintCallable)
	virtual void LightAttack();
	
	virtual void BasicAttack();
	virtual void SpecialAttack();
	void LoadPower(int InflictedDamage);
	void UnloadPower(int DamageTaken);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;
};
