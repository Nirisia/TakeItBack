// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "DA_Spawner.h"
#include "EnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    SetRootComponent(Mesh);

    Target = CreateDefaultSubobject<UBoxComponent>(TEXT("Target"));
    Target->SetupAttachment(Mesh);
    Target->SetCollisionProfileName(TEXT("NoCollision"));
    Target->SetGenerateOverlapEvents(false);
}

void AEnemySpawner::SpawnWave(int Index)
{
    FActorSpawnParameters SpawnParam;
    SpawnParam.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    auto SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(WaveList[Index].Enemy.Get(),
                                                                Target->GetComponentTransform(), SpawnParam);
    SpawnedEnemy->SpawnDefaultController();
    SpawnedEnemy->Spawner = this;

    SpawnedList.Add(SpawnedEnemy);
}

void AEnemySpawner::OnEnemyDie(AEnemyCharacter* DeadEnemy)
{
    for (int i = 0; i < SpawnedList.Num(); i++)
    {
        if (DeadEnemy == SpawnedList[i])
        {
            SpawnedList.RemoveAt(i);
        }
    }
}

void AEnemySpawner::MyTakeDamage(int Damaged)
{
    if (Life > 0)
    {
        Life -= Damaged;
    }
    if (Life <= 0)
    {
        Destroy();
    }
}

void AEnemySpawner::LoadDataAssets()
{
    if (SpawnerData)
    {
        WaveList = SpawnerData->WaveList;
        Life = SpawnerData->Life;
        bIsLoop = SpawnerData->IsLoop;
        MaxEnemyAlive = SpawnerData->MaxEnemyAlive;
        Mesh->SetStaticMesh(SpawnerData->Mesh);
        DistanceToStopSpawn = SpawnerData->DistanceToStopSpawn;
        DistanceToActivate = SpawnerData->DistanceToActivate;
    }
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
    Super::BeginPlay();
    LoadDataAssets();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!PlayerCharacter)
    {
        PlayerCharacter = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    }

    if (PlayerCharacter)
    {
        const FVector PlayerOffset = PlayerCharacter->GetActorLocation() - GetActorLocation();
        const float Distance = PlayerOffset.Size();

        bIsActive = (Distance <= DistanceToActivate) && (Distance >= DistanceToStopSpawn);


        if (bIsActive && CurrentWaveIndex < WaveList.Num() && SpawnedList.Num() < MaxEnemyAlive)
        {
            if (TimerWave >= WaveList[CurrentWaveIndex].Timer)
            {
                SpawnWave(CurrentWaveIndex);
                CurrentWaveIndex++;
                TimerWave = 0;
            }
        }
        else if (CurrentWaveIndex >= WaveList.Num() && bIsLoop)
        {
            CurrentWaveIndex = 0;
        }

        TimerWave += DeltaTime;
    }
}
