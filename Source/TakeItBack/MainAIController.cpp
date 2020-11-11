// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAIController.h"

#include "DA_AIController.h"
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
    LoadDataAssets();

    FAISenseID Id = UAISense::GetSenseID(UAISense_Sight::StaticClass());
 
    if (!Id.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Wrong Sense ID"));
        return;
    }
 
    // GetPerception() = AIController->GetPerceptionComponent()
    const auto Config = GetPerceptionComponent()->GetSenseConfig(Id);
    if (Config)
    {
        SightConfig = Cast<UAISenseConfig_Sight>(Config);
        SightConfig->SightRadius = SightRadius;
        SightConfig->LoseSightRadius = LoseSightRadius;
        SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
        SightConfig->AutoSuccessRangeFromLastSeenLocation = AutoSuccessRangeFromLastSeenLocation;
    }

 
    GetPerceptionComponent()->RequestStimuliListenerUpdate();
}

void AMainAIController::OnPossess(APawn* P)
{
    Super::OnPossess(P);
    if (Btree)
    {
        RunBehaviorTree(Btree);
        Blackboard = GetBlackboardComponent();
        if (Blackboard)
        {
            Blackboard->SetValueAsFloat("DistanceToAttack", DistanceToAttack);
        }
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
        if (Blackboard)
        {
            if (Blackboard->GetValueAsBool("CanSeePlayer"))
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
    if (Cast<APlayerCharacter>(Actor) && Blackboard)
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
    SightConfig->SightRadius = SightRadius;
    SightConfig->LoseSightRadius = LoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
    SightConfig->SetMaxAge(5.f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = AutoSuccessRangeFromLastSeenLocation;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    //Add sight configuration component to perception component
    GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
    GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMainAIController::OnTargetDetected);
    GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AMainAIController::LoadDataAssets()
{
    if (ControllerData)
    {
        DistanceToAttack = ControllerData->DistanceToAttack;
        SightRadius = ControllerData->SightRadius;
        LoseSightRadius = ControllerData->LoseSightRadius;
        PeripheralVisionAngleDegrees = ControllerData->PeripheralVisionAngleDegrees;
        AutoSuccessRangeFromLastSeenLocation = ControllerData->AutoSuccessRangeFromLastSeenLocation;
    }
}
