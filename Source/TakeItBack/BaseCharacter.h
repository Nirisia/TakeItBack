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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxLife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentLife;

	/** Walkspeed of the Character, in meters per seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 600.f;

	/** Initial Velocity of the Character when jumping, in meters per seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHeight = 200.f;

	/** Air Control of the Character. It's a percentage of the default WalkSpeed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AirControl = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GravityScale = 1.0f;

	/** Maximum rotation speed of the character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RotationRate = FRotator(0.0f, 540.f, 0.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bImpact = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsDead = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DataAssets", meta = (AllowPrivateAccess = "true"))
	class UDA_BaseCharacter* CharacterData;

	UFUNCTION(BlueprintCallable)
    virtual void LoadDataAssets();
	
	ABaseCharacter();

	UFUNCTION(BlueprintCallable)
	virtual void Attack();
	
	UFUNCTION(BlueprintCallable)
	virtual int MyTakeDamage(int Damage);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetWeaponCollision(bool bGenerateOverlap);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
