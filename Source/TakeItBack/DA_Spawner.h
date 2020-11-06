// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StaticMesh.h"

#include "DA_Spawner.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API UDA_Spawner : public UDataAsset
{
	GENERATED_BODY()

	public:
	
	UPROPERTY(EditAnywhere, Category="Stats")
	TArray<TSubclassOf<class AEnemyCharacter>> EnemyList;
	
	//UPROPERTY(EditAnywhere, Category="Stats")
	//TArray<TArray<TSubclassOf<class AEnemyCharacter>>> WaveList;

	UPROPERTY(EditAnywhere, Category="Stats")
	int Life;

	UPROPERTY(EditAnywhere, Category="Stats")
	bool IsActive;

	UPROPERTY(EditAnywhere, Category="Stats")
	bool IsLoop;

	UPROPERTY(EditAnywhere, Category="Stats")
	float TimeBetweenWave;

	UPROPERTY(EditAnywhere, Category="Stats")
	class UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, Category="Stats")
	FBox Target;
};
