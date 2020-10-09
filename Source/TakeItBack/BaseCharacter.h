// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon.h"
#include "BaseCharacter.generated.h"

UCLASS()
class TAKEITBACK_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxLife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentLife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AtkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	bool bImpact = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	bool bIsAttacking = false;
	
	ABaseCharacter();
	virtual void Attack();
	virtual void MyTakeDamage(AWeapon* AttackingWeapon);
	void SetWeaponCollision(bool bGenerateOverlap);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
