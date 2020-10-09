// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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
	APlayerCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	bool bIsAxe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	bool bCanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	bool bIsBlocking;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	AWeapon* Sword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	AWeapon* Axe;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AtkCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ChangeWeaponAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> AxeAttacksAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> SwordAttacksAnim;
	
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
	void ChangeWeapon();
	void Attack() override;
	void SpecialAttack();
	void Defense();
	void StopDefense();
	void Jump() override;
	void StopJumping() override;
	
	protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
};
