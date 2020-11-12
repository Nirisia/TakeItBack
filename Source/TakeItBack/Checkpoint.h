// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class TAKEITBACK_API ACheckpoint : public AActor
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Checkpoint", meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* TriggerBox;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Checkpoint", meta = (AllowPrivateAccess = "true"))
    class UBillboardComponent* RespawnPointBillboard;

public:
    // Sets default values for this actor's properties
    ACheckpoint();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Checkpoint")
    FVector RespawnPoint = FVector(0);

    UFUNCTION(BlueprintCallable)
    FVector GetRespawnLocation();


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor*
                                 OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool
                                 bFromSweep, const FHitResult& SweepResult);
};
