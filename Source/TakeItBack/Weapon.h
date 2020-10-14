// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Engine/StaticMesh.h"
#include "Weapon.generated.h"



UCLASS(meta=(BlueprintSpawnableComponent), Blueprintable)
class TAKEITBACK_API AWeapon : public AActor
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Mesh")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Mesh")
	class UBoxComponent* BoxComponent;

	UPROPERTY()
	class ABaseCharacter* OwnerCharacter;
	
	AWeapon();
	
	UFUNCTION(BlueprintCallable)
	virtual void LightAttack();
	
	UFUNCTION(BlueprintCallable)
	virtual void SpecialAttack();
	
	UFUNCTION(BlueprintCallable)
	void LoadPower(int InflictedDamage);
	
	UFUNCTION(BlueprintCallable)
	void UnloadPower(int DamageTaken);

	UFUNCTION()
	void SetWeaponCollision(bool bGenerateOverlap);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
