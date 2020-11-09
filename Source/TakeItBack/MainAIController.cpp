// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAIController.h"
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
#include "Engine/Engine.h"

AMainAIController::AMainAIController() : Super()
{
    SetupPerceptionSystem();
}

void AMainAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AMainAIController::OnPossess(APawn* P)
{
    Super::OnPossess(P);
    if (Btree)
    {
        RunBehaviorTree(Btree);
        Blackboard = GetBlackboardComponent();
    }
}

void AMainAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	
    AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn());
    if (Player && Enemy)
    {
        if (Enemy->bIsDead)
        {
            UnPossess();
        }
        
        Blackboard = GetBlackboardComponent();
        if(Blackboard)
        {
            if(Blackboard->GetValueAsBool("CanSeePlayer"))
            {
                FRotator NewRotation = (Player->GetActorLocation() - Enemy->GetActorLocation()).Rotation();
                NewRotation.Pitch = 0.f;
                SetControlRotation(NewRotation);
            }
            
            float Distance = Enemy->GetDistanceTo(Player);
            Blackboard->SetValueAsFloat("DistanceToPlayer", Distance);
            Blackboard->SetValueAsBool("CanAttack", Enemy->bCanAttack);
        }
    }
    else
    {
        Player = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    }
}

// FRotator AMainAIController::GetControlRotation() const
// {
//     if (GetPawn() == nullptr)
//     {
//         return FRotator(0.0f, 0.0f, 0.0f);
//     }
//     return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
// }

void AMainAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
    if(Cast<APlayerCharacter>(Actor) && Blackboard)
    {
        Blackboard->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
    }
}

void AMainAIController::SetupPerceptionSystem()
{
    
    //Create and initialise sight configuration object
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
    SetPerceptionComponent(*PerceptionComponent);
    SightConfig->SightRadius = 1000.f;
    SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.f;
    SightConfig->PeripheralVisionAngleDegrees = 360.f;
    SightConfig->SetMaxAge(5.f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 900.f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    //Add sight configuration component to perception component
    GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
    GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMainAIController::OnTargetDetected);
    GetPerceptionComponent()->ConfigureSense(*SightConfig);
}
