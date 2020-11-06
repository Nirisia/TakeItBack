// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	float CameraElapsedTime;

	UPROPERTY()
	float AutoCameraSensitivity;
	
	UPROPERTY(EditDefaultsOnly, Category="DataAssets")
	class UDA_PlayerController* ControllerData;

	UFUNCTION(BlueprintCallable)
	void LoadDataAssets();

	UFUNCTION()
	void CameraTick(float DeltaSeconds);

public:
	UPROPERTY()
	float BaseFOV;

	UPROPERTY()
	float TimeBeforeAutoCamera = 2.0f;

	
	virtual void Tick(float DeltaSeconds) override;
	AMainPlayerController();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};
