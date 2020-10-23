// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon", meta = (AllowPrivateAccess = "true"))
    class UChildActorComponent* Weapon;
	
	public:
	AEnemyCharacter();
	virtual void SetWeaponCollision(bool bGenerateOverlap) override;
	virtual void Attack() override;
	
	UFUNCTION(BlueprintCallable)
    void ValidateAttack();
};
