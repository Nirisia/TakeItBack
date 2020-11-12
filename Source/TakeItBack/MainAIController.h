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

	UFUNCTION()
	void LoadDataAssets();
	
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* Btree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataAsset", meta = (AllowPrivateAccess = "true"))
	class UDA_AIController* ControllerData;

	class ABaseCharacter* Player;

	class UBlackboardComponent* Blackboard;

	float DistanceToAttack = 150.f;

	float SightRadius = 1000.f;
	float LoseSightRadius = 1050.f;;
	float PeripheralVisionAngleDegrees = 360.f;
	float AutoSuccessRangeFromLastSeenLocation = 900.f;

};
