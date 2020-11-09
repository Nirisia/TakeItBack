// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TAKEITBACK_API UBTTAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTTAttack(FObjectInitializer const& ObjectInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
