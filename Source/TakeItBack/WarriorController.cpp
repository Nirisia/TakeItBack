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
#include "PlayerCharacter.h"

AWarriorController::AWarriorController(FObjectInitializer const& object_initializer)
{
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/ThirdPersonCPP/Blueprints/Enemies/WarriorBT.WarriorBT'"));

    if (obj.Succeeded())
    {
        Btree = obj.Object;
    }
    behavior_tree_component = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
    Blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
    SetupPerceptionSystem();
}

void AWarriorController::BeginPlay()
{
    Super::BeginPlay();
    RunBehaviorTree(Btree);
    behavior_tree_component->StartTree(*Btree);
    //UseBlackboard(blackboard,blackboard)
}

void AWarriorController::OnPossess(APawn* P)
{
    Super::OnPossess(P);
    if (Blackboard)
    {
        Blackboard->InitializeBlackboard(*Btree->BlackboardAsset);
    }
}

void AWarriorController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	
    ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn());

    FRotator NewRotation = (Player->GetActorLocation() - GetPawn()->GetActorLocation()).Rotation();
    NewRotation.Pitch = 0.f;
    GetPawn()->SetActorRotation(NewRotation);
    float Distance = GetPawn()->GetDistanceTo(Player);
    GetBlackboard()->SetValueAsFloat("DistanceToPlayer", Distance);
    GetBlackboard()->SetValueAsBool("CanAttack", Enemy->bCanAttack);
}

UBlackboardComponent* AWarriorController::GetBlackboard() const
{
    return Blackboard;
}

FRotator AWarriorController::GetControlRotation() const
{
    if (GetPawn() == nullptr)
    {
        return FRotator(0.0f, 0.0f, 0.0f);
    }
    return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AWarriorController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
    if(auto const ch = Cast<APlayerCharacter>(Actor))
    {
        GetBlackboard()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
    }
}

void AWarriorController::SetupPerceptionSystem()
{
    //Create and initialise sight configuration object
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
    SightConfig->SightRadius = 500.f;
    SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.f;
    SightConfig->PeripheralVisionAngleDegrees = 90.f;
    SightConfig->SetMaxAge(5.f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 900.f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    //Add sight configuration component to perception component
    GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
    GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AWarriorController::OnTargetDetected);
    GetPerceptionComponent()->ConfigureSense(*SightConfig);
}
