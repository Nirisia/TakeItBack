// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DA_Spawner.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class TAKEITBACK_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();
	
	void SpawnWave(int Index);

	void OnEnemyDie(class AEnemyCharacter* DeadEnemy);

	int MyTakeDamage(int Damage);

	UPROPERTY(EditAnywhere, Category="Stats")
	TArray<FWave> WaveList;

	UPROPERTY(EditAnywhere, Category="Stats")
	int Life;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stats")
	bool bIsActive;

	UPROPERTY(EditAnywhere, Category="Stats")
	bool bIsLoop;

	UPROPERTY(EditAnywhere, Category="Stats")
	int MaxEnemyAlive;

	UPROPERTY()
	int CurrentWaveIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	float DistanceToStopSpawn = 300.f;

	UPROPERTY(BlueprintReadOnly)
	float DistanceToActivate = 3000.f;
	
	UPROPERTY(BlueprintReadOnly)
	float TimerWave = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="DataAssets", meta = (AllowPrivateAccess = "true"))
	class UDA_Spawner* SpawnerData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Stats", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Stats", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY()
	class ABaseCharacter* PlayerCharacter;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSpawned(FTransform EnemyTransform);

private:
	UFUNCTION(BlueprintCallable)
    virtual void LoadDataAssets();
	
	UPROPERTY()
	TArray<AEnemyCharacter*> SpawnedList;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
