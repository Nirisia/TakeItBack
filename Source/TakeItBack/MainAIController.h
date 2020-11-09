// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MainAIController.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API AMainAIController : public AAIController
{
	GENERATED_BODY()

	AMainAIController();

	public:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* P) override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
    void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
	void SetupPerceptionSystem();

	private:
	
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "AI", meta = (AllowprivateAccess = "true"))
	class UBehaviorTree* Btree;

	class ABaseCharacter* Player;

	class UBlackboardComponent* Blackboard;
};
