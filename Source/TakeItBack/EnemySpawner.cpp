// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include <string>

#include "DA_Spawner.h"
#include "EnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"


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

	//for (int i = 0; i < WaveList[Index].Num(); i++)
	for (int i = 0; i < EnemyList.Num(); i++)
	{
		auto SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyList[i].Get(), Target->GetComponentTransform(), SpawnParam);
        SpawnedEnemy->SpawnDefaultController();
        SpawnedEnemy->Spawner = this;
		EnemySpawned.Add(SpawnedEnemy);	
	}
}

void AEnemySpawner::OnEnemyDie(AEnemyCharacter* DeadEnemy)
{
	for (int i = 0; i < EnemySpawned.Num(); i++)
	{
		if(DeadEnemy == EnemySpawned[i])
		{
			EnemySpawned.RemoveAt(i);
		}
	}
}

void AEnemySpawner::Damaged(int Damaged)
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
	if(SpawnerData)
	{
		EnemyList = SpawnerData->EnemyList;
		//WaveList = SpawnerData->WaveList;
		Life = SpawnerData->Life;
		IsActive = SpawnerData->IsActive;
		IsLoop = SpawnerData->IsLoop;
		TimeBetweenWave = SpawnerData->TimeBetweenWave;
		Mesh->SetStaticMesh(SpawnerData->Mesh);
		Target->SetBoxExtent(SpawnerData->Target.GetExtent());
	}
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	LoadDataAssets();
	//SpawnWave(0);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimerWave += DeltaTime;
	int cpt = 1;

	if (TimerWave >= TimeBetweenWave)
	{
		TimerWave = 0;
		SpawnWave(0);
		/*if (cpt < WaveList.Num())
		{
			SpawnWave(cpt);
			cpt++;
		}
		if (cpt >= WaveList.Num() && IsLoop)
		{
			cpt = 0;
		}*/
	}
}

