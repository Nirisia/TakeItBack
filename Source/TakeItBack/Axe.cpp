// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "Engine.h"

UAxe::UAxe()
{
}

void UAxe::LightAttack()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AxeAttack"));
}

void UAxe::BasicAttack()
{
}

void UAxe::SpecialAttack()
{
}
