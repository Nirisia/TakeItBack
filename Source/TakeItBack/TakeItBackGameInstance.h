// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TakeItBackGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TAKEITBACK_API UTakeItBackGameInstance : public UGameInstance
{
	GENERATED_BODY()
	public:
	UPROPERTY(BlueprintReadWrite)
	bool bInverseXAxis;

	UPROPERTY(BlueprintReadWrite)
	bool bInverseYAxis;

};
