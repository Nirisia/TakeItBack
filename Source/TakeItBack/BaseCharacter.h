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
	/** Max Life of the Character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Characteristics")
	int MaxLife;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	int CurrentLife;

	/** Walkspeed of the Character, in meters per seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Characteristics")
	float WalkSpeed = 600.f;

	/** Initial Velocity of the Character when jumping, in meters per seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Characteristics")
	float JumpSpeed = 600.f;

	/** Air Control of the Character. It's a percentage of the default WalkSpeed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Characteristics")
	float AirControl = 0.2f;

	/** Maximum rotation speed of the character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Characteristics")
	float RotationSpeed = 540.f;

	/** Speed of the Attack Animation, in percentage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Characteristics")
	float AtkSpeed = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bImpact = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsAttacking = false;
	
	ABaseCharacter();

	UFUNCTION(BlueprintCallable)
	virtual void Attack();
	UFUNCTION(BlueprintCallable)
	virtual void MyTakeDamage(AWeapon* AttackingWeapon);
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(bool bGenerateOverlap);

	UFUNCTION(BlueprintNativeEvent)
	void SetWeaponCollisionEvent(bool bGenerateOverlap);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
