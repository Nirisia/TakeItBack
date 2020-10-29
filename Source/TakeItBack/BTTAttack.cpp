// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTAttack.h"

#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "WarriorController.h"

UBTTAttack::UBTTAttack(FObjectInitializer const& object_initializer)
{
    NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTAttack::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
    ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    auto const cont = Cast<AWarriorController>(owner_comp.GetAIOwner());
    AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(cont->GetPawn());

    if (cont->GetBlackboard()->GetValueAsFloat("DistanceToPlayer") < 150)
    {
        Enemy->Attack();
    }

    FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}
