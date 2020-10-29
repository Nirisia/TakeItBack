// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTMoveToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "WarriorController.h"

UBTTMoveToPlayer::UBTTMoveToPlayer(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Move To Player");
}

EBTNodeResult::Type UBTTMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    auto const cont = Cast<AWarriorController>(OwnerComp.GetAIOwner());

    FVector const PlayerLocation = Player->GetActorLocation();
    FVector Dir = (PlayerLocation - cont->GetPawn()->GetActorLocation());
    float Fact = Dir.Size() - Radius;
    Dir.Normalize();
    FVector loc = Dir * Fact + cont->GetPawn()->GetActorLocation();
    
    OwnerComp.GetAIOwner()->MoveToLocation(loc);
    cont->GetBlackboard()->SetValueAsVector("Follow", loc);

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}
