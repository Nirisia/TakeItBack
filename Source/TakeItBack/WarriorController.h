// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "WarriorController.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API AWarriorController : public AAIController
{
	GENERATED_BODY()

	AWarriorController(FObjectInitializer const& object_initializer = FObjectInitializer::Get());

	void BeginPlay() override;
	void OnPossess(APawn* P) override;
	void Tick(float DeltaSeconds) override;
	class UBlackboardComponent* GetBlackboard() const;
	virtual FRotator GetControlRotation() const override;

	private:
	UPROPERTY(EditInstanceOnly, BluePrintReadWrite, Category = "AI", meta = (AllowprivateAccess = "true"))
	class UBehaviorTreeComponent* behavior_tree_component;

	UPROPERTY(EditInstanceOnly, BluePrintReadWrite, Category = "AI", meta = (AllowprivateAccess = "true"))
	class UBehaviorTree* btree;

	class UBlackboardComponent* blackboard;

	class UAISenseConfig_Sight* sight_config;	
};
