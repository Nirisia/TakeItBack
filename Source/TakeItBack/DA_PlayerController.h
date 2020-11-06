// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API UDA_PlayerController : public UDataAsset
{
	GENERATED_BODY()
public:

	/* Initial rotation applied to the controller at BeginPlay*/
	UPROPERTY(EditAnywhere, Category = "Camera")
	FRotator InitialControllerRotation = FRotator(0.f, 0.f, 0.f);

	/* Rotation of the Camera*/
	UPROPERTY(EditAnywhere, Category = "Camera")
	FRotator CameraRotation = FRotator(-20.f, 0.f, 0.f);

	/* Maximum angle of the controller when looking DOWN */
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ViewPitchMin = -45.f;

	/* Maximum angle of the controller when looking UP */
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ViewPitchMax = 70.f;

	/* Default FOV of the Camera*/
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraFOV = 90.f;

	/* LagSpeed of the camera. Lower is slower. Zero deactivate the lag. */
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraLagSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float TimeBeforeAutoCamera = 2.0f;

	/* Sensitivity of the camera when auto-rotating */
	UPROPERTY(EditAnywhere, Category = "Camera")
	float AutoCameraSensitivity = 1.f;
};
