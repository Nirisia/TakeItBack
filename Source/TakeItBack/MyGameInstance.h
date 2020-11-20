// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InverseXAxis = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InverseYAxis = false;	
};
