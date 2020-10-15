// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "Engine/StaticMesh.h"
#include "Weapon.generated.h"



UCLASS(meta=(BlueprintSpawnableComponent), Blueprintable)
class TAKEITBACK_API AWeapon : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	int AtkCount = 0;
	
public:	
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	int MaxPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	int Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	int Damage;

	UPROPERTY()
	int i = 0; //Iteration for passive bonus

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float AtkSpeed = 1.f;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	TArray<class UAnimMontage*> AttacksAnim;

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

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(bool bGenerateOverlap);

	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
