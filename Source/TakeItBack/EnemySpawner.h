// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

USTRUCT()
struct FWave
{
	GENERATED_BODY()
	TArray<TSubclassOf<class AEnemyCharacter>> EnemyList;
    float Timer;
};

UCLASS()
class TAKEITBACK_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();
	
	void SpawnWave(int Index);

	void OnEnemyDie(class AEnemyCharacter* DeadEnemy);

	void Damaged(int Damage);
	
	UPROPERTY(EditAnywhere, Category="Stats")
	TArray<TSubclassOf<class AEnemyCharacter>> EnemyList;

	//UPROPERTY(EditAnywhere, Category="Stats")
	//TArray<FWave> WaveList;

	UPROPERTY(EditAnywhere, Category="Stats")
	int Life;

	UPROPERTY(EditAnywhere, Category="Stats")
	bool IsActive;

	UPROPERTY(EditAnywhere, Category="Stats")
	bool IsLoop;

	UPROPERTY(EditAnywhere, Category="Stats")
	float TimeBetweenWave;

	UPROPERTY()
	float TimerWave = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DataAssets", meta = (AllowPrivateAccess = "true"))
	class UDA_Spawner* SpawnerData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Stats", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Stats", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

private:
	UFUNCTION(BlueprintCallable)
    virtual void LoadDataAssets();
	
	UPROPERTY()
	TArray<AEnemyCharacter*> EnemySpawned;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
