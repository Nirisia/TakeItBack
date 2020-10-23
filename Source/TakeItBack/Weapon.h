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
	int MaxPower = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	int Power = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	int Damage = 5;

	/* */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	int BonusStack = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsSpecialAttackActive = false;

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
    int AtkCount = 0;
	
	AWeapon();
	
	UFUNCTION(BlueprintCallable)
	virtual void LightAttack();
	
	UFUNCTION(BlueprintCallable)
	virtual void SpecialAttack();

	UFUNCTION(BlueprintCallable)
	virtual void Defense();

	UFUNCTION(BlueprintCallable)
	virtual void StopDefense();
	
	UFUNCTION(BlueprintCallable)
	virtual void LoadPower(int InflictedDamage);
	
	UFUNCTION(BlueprintCallable)
	virtual void UnloadPower(int DamageTaken);

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(bool bGenerateOverlap);


	
	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	class ABaseCharacter* GetParentCharacter();

	UFUNCTION()
	virtual void AttackCollision(UPrimitiveComponent* OverlappedComponent, AActor* 
			OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool 
			bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;
};
