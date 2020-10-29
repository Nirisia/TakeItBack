// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
    MaxLife = 100;
    CurrentLife = 100;
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    Super::LoadDataAssets();
}
