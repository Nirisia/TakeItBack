// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "Engine.h"

ASword::ASword()
{
}

void ASword::LightAttack()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SwordAttack"));

}

void ASword::SpecialAttack()
{
    ShieldMeteor();
}

void ASword::ShieldMeteor_Implementation()
{
    
}
