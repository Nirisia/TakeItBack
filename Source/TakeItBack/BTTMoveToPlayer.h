// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTMoveToPlayer.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TAKEITBACK_API UBTTMoveToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 100.f;
	
	UBTTMoveToPlayer(FObjectInitializer const& ObjectInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
