// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTMoveToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "MainAIController.h"

UBTTMoveToPlayer::UBTTMoveToPlayer(FObjectInitializer const& ObjectInitializer)
{
    NodeName = TEXT("Move To Player");
}

EBTNodeResult::Type UBTTMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AAIController* const AIController = Cast<AAIController>(OwnerComp.GetAIOwner());

    FVector const PlayerLocation = Player->GetActorLocation();
    FVector Direction = (PlayerLocation - AIController->GetPawn()->GetActorLocation());
    const float Fact = Direction.Size() - Radius;
    Direction.Normalize();
    const FVector TargetLocation = Direction * Fact + AIController->GetPawn()->GetActorLocation();
    
    OwnerComp.GetAIOwner()->MoveToLocation(TargetLocation);
    AIController->GetBlackboardComponent()->SetValueAsVector("Follow", TargetLocation);

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}
