// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UENUM()
enum EInputType
{
    None = 0,
    Attack,
    SpecialAttack,
    Defense,
    ChangeWeapon,
    Jump
};

USTRUCT()
struct FInputBuffer
{
    GENERATED_BODY()
    EInputType Type = None;
    bool bIsValid = false;
};

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

    UPROPERTY()
    FRotator InitialControllerRotation;

    UPROPERTY(EditDefaultsOnly, Category="DataAssets")
    class UDA_PlayerController* ControllerData;

    UPROPERTY()
    FInputBuffer InputBuffer;

    UFUNCTION(BlueprintCallable)
    void LoadDataAssets();

    UFUNCTION()
    void CameraTick(float DeltaSeconds);

    UFUNCTION()
    void InvalidateInput();

    UFUNCTION()
    void BufferInput(EInputType InputType);

    UFUNCTION()
    void Attack();

    UFUNCTION()
    void SpecialAttack();

    UFUNCTION()
    void ChangeWeapon();

    UFUNCTION()
    void Defense();

    UFUNCTION()
    void Jump();

    /* Will try to consume the input and apply the logic.
     * Set InputBuffer.bIsValid to true if input consumed, else false. */
    UFUNCTION()
    void TryConsumeInput();

    UPROPERTY()
    class APlayerCharacter* PlayerCharacter;

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
