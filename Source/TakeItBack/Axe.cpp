// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "Engine.h"

AAxe::AAxe()
{
}

void AAxe::LightAttack()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AxeAttack"));
}

void AAxe::SpecialAttack()
{
}
