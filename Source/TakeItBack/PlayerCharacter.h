// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "DA_Player.h"

#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	UCameraComponent* GetFollowCamera() const
	{
		return FollowCamera;
	}

	class USpringArmComponent* GetCameraBoom() const
	{
		return CameraBoom;
	}

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent* Sword;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent* Axe;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Shield;

	UFUNCTION(BlueprintCallable)
	void LoadWeaponStats();

	virtual void LoadDataAssets() override;	

public:
	class UAIPerceptionStimuliSourceComponent* Stimulus;
	void SetupStimulus();
	
	APlayerCharacter();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHeal();

	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeWeapon(bool IsAxe);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnRespawn();

protected:
	virtual void BeginPlay() override;
	
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAxe = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanSpecialAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanDefend = true;

	bool bInvulnerable = false;

	// virtual int MyTakeDamage(int Damage, EWeaponResistance WeaponType, FVector Direction) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanChangeWeapon = true;

	/** Speed of the ChangeWeapon Animation, in percentage. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ChangeWeaponSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int StackLimit = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ChangeWeaponAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ACheckpoint* Checkpoint;
	
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	* Called via input to turn at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);


	
	/**
	* Called via input to turn look up/down at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);
	
	UFUNCTION(BlueprintCallable)
	void ChangeWeapon();

	UFUNCTION(BlueprintCallable)
	void SwapMeshes();
	
	virtual void Attack() override;
	
	virtual void ResetCombo() override;

	UFUNCTION(BlueprintCallable)
	void SpecialAttack();

	UFUNCTION(BlueprintCallable)
	void Defense();
	UFUNCTION(BlueprintCallable)
	void StopDefense();

	virtual class AWeapon* GetCurrentWeapon() override;

	UFUNCTION(BlueprintCallable)
	void SetInvulnerable(bool bInvunerable);

	UFUNCTION(BlueprintCallable)
	void Heal(float HealPercent);

	virtual void Die() override;
	virtual void Revive() override;

	virtual void Stun() override;
	virtual void EndStun() override;
	
	UFUNCTION(BlueprintCallable)
	void Respawn();
	
	UFUNCTION()
	void SetShieldMesh(class UStaticMesh* ShieldMesh);
};
