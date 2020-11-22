// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UENUM()
enum class EWeaponResistance : uint8
{
	None,
    Axe,
    Sword
};

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReceiveVelocity = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int KnockbackCoeffDmg = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ResistanceCoeff;

	UPROPERTY(EditAnywhere)
	EWeaponResistance Resistance;

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
	bool bIsDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="DataAssets", meta = (AllowPrivateAccess = "true"))
	class UDA_BaseCharacter* CharacterData;

	UPROPERTY(EditAnywhere, Category="Anim")
	class UAnimMontage* StunAnim;

	UFUNCTION(BlueprintCallable)
    virtual void LoadDataAssets();
	
	ABaseCharacter();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFootstep();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTakeDamage();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDie();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStun();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndStun();
	
	UFUNCTION(BlueprintCallable)
	virtual void Die();

	UFUNCTION(BlueprintCallable)
    virtual void Revive();

	UFUNCTION(BlueprintCallable)
    virtual void Stun();

	UFUNCTION(BlueprintCallable)
    virtual void EndStun();

	UFUNCTION(BlueprintCallable)
    void TriggerFootstep();
	
	UFUNCTION(BlueprintCallable)
	virtual void Attack();
	
	UFUNCTION(BlueprintCallable)
	virtual int MyTakeDamage(int Damage, EWeaponResistance WeaponType = EWeaponResistance::None, FVector AttackLocation = FVector::ZeroVector);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetWeaponCollision(bool bGenerateOverlap);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual class AWeapon* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void ValidateAttack();
	
	UFUNCTION(BlueprintCallable)
	virtual void ResetCombo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
