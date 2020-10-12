// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Engine.h"

USword::USword()
{
}

void USword::LightAttack()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SwordAttack"));

}

void USword::BasicAttack()
{
}

void USword::SpecialAttack()
{
}
