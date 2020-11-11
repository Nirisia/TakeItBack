// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_AIController.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API UDA_AIController : public UDataAsset
{
    GENERATED_BODY()

public:
    /* Radius at which the enemy will begin to attack the player*/
    UPROPERTY(EditAnywhere, Category="Properties")
    float DistanceToAttack = 150.f;

    /* Radius at which the enemy will begin to see the player*/
    UPROPERTY(EditAnywhere, Category="Sight")
    float SightRadius = 1000.f;

    /* Radius at which the enemy will lose sight of the player */
    UPROPERTY(EditAnywhere, Category="Sight")
    float LoseSightRadius = 1050.f;;

    /* Angle of vision of the enemy */
    UPROPERTY(EditAnywhere, Category="Sight")
    float PeripheralVisionAngleDegrees = 360.f;

    /* Range at which the enemy will always spot the player if he was already seen before not long ago. */
    UPROPERTY(EditAnywhere, Category="Sight")
    float AutoSuccessRangeFromLastSeenLocation = 900.f;
};
