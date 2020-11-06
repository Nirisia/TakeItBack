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

	virtual void LoadDataAssets() override;	

public:
	class UAIPerceptionStimuliSourceComponent* Stimulus;
	void SetupStimulus();
	
	APlayerCharacter();


protected:
	virtual void BeginPlay() override;
	
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAxe = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanSpecialAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanDefend = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanChangeWeapon = true;

	/** Speed of the ChangeWeapon Animation, in percentage. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ChangeWeaponSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AttackSpeedCoeff = 0.7f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int StackLimit = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ChangeWeaponAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool keys = false;
	
public:
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
	
	UFUNCTION(BlueprintCallable)
	void ValidateAttack();
	
	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	UFUNCTION(BlueprintCallable)
	void SpecialAttack();

	UFUNCTION(BlueprintCallable)
	void Defense();
	UFUNCTION(BlueprintCallable)
	void StopDefense();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeapon* GetCurrentWeapon();

	virtual void SetWeaponCollision(bool bGenerateOverlap) override;

	UFUNCTION(BlueprintCallable)
	void Heal(float HealPercent);
	
	UFUNCTION()
	void SetShieldMesh(class UStaticMesh* ShieldMesh);
};
