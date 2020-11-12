// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StaticMesh.h"

#include "DA_Spawner.generated.h"

USTRUCT()
struct FWave
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEnemyCharacter> Enemy;

	UPROPERTY(EditDefaultsOnly)
	float Timer;
};

/**
 * 
 */
UCLASS()
class TAKEITBACK_API UDA_Spawner : public UDataAsset
{
	GENERATED_BODY()

	public:
	
	// UPROPERTY(EditAnywhere, Category="Stats")
	// TArray<TSubclassOf<class AEnemyCharacter>> EnemyList;
	
	UPROPERTY(EditAnywhere, Category="Stats")
	TArray<FWave> WaveList;

	UPROPERTY(EditAnywhere, Category="Stats")
	int Life;

	UPROPERTY(EditAnywhere, Category="Stats")
	bool IsLoop;

	UPROPERTY(EditAnywhere, Category="Stats")
	int MaxEnemyAlive = 5;

	UPROPERTY(EditAnywhere, Category="Stats")
	float DistanceToStopSpawn = 300.f;

	UPROPERTY(EditAnywhere, Category="Stats")
	float DistanceToActivate = 3000.f;
	
	UPROPERTY(EditAnywhere, Category="Stats")
	class UStaticMesh* Mesh;
};
