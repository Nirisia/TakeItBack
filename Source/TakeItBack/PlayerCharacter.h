// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
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

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent* Sword;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	class UChildActorComponent* Axe;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Shield;
	
	public:
	APlayerCharacter();


protected:
	virtual void BeginPlay() override;
public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Camera")
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bIsAxe = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bCanSpecialAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bCanDefend = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="State")
	bool bCanChangeWeapon = true;

	/** Speed of the ChangeWeapon Animation, in percentage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Characteristics")
	float ChangeWeaponSpeed = 1.0f;
	


	/** Offset of the camera. Automatically set the CameraBoom length and offset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	FVector CameraOffset = FVector(400.f, 0.f, 200.f);

	/** Pitch rotation of the Camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	float CameraAngle = -20;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ChangeWeaponAnim;
	
	protected:

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

public:
	virtual void SetWeaponCollision(bool bGenerateOverlap) override;
	protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
};
