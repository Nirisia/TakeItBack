// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTAttack.h"

#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "MainAIController.h"

UBTTAttack::UBTTAttack(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* const AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
    AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(AIController->GetPawn());
    const UBlackboardComponent* AIBlackBoard = AIController->GetBlackboardComponent();
    if (AIBlackBoard->GetValueAsFloat("DistanceToPlayer") < AIBlackBoard->GetValueAsFloat("DistanceToAttack"))
    {
       Enemy->Attack();
    }

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}
