// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "EnemyCharacter.h"

AWarriorController::AWarriorController(FObjectInitializer const& object_initializer)
{
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/WarriorBT.WarriorBT'"));

    if (obj.Succeeded())
    {
        btree = obj.Object;
    }
    behavior_tree_component = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
    blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
}

void AWarriorController::BeginPlay()
{
    Super::BeginPlay();
    RunBehaviorTree(btree);
    behavior_tree_component->StartTree(*btree);
    //UseBlackboard(blackboard,blackboard)
}

void AWarriorController::OnPossess(APawn* P)
{
    Super::OnPossess(P);
    if (blackboard)
    {
        blackboard->InitializeBlackboard(*btree->BlackboardAsset);
    }
}

void AWarriorController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	
    ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn());

    GetPawn()->SetActorRotation((Player->GetActorLocation() - GetPawn()->GetActorLocation()).Rotation());
    float Distance = GetPawn()->GetDistanceTo(Player);
    GetBlackboard()->SetValueAsFloat("DistanceToPlayer", Distance);
    GetBlackboard()->SetValueAsVector("PlayerPos", Player->GetActorLocation());
    GetBlackboard()->SetValueAsBool("CanAttack", Enemy->bCanAttack);
}

UBlackboardComponent* AWarriorController::GetBlackboard() const
{
    return blackboard;
}

FRotator AWarriorController::GetControlRotation() const
{
    if (GetPawn() == nullptr)
    {
        return FRotator(0.0f, 0.0f, 0.0f);
    }
    return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}
