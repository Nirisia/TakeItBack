// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"



#include "BaseCharacter.h"
#include "Bow.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 1000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->BodyInstance.SetCollisionProfileName("OverlapAllDynamic");
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->MoveIgnoreActors.Add(GetOwner());
	RootComponent = BoxComponent;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComponent->SetupAttachment(BoxComponent);

}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnBeginOverlap);

}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArrow::SetDamage(int NewDamage)
{
	Damage = NewDamage;
}

void AArrow::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                   const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner())
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);
		if (Character)
		{
			int InflictedDamage = Character->MyTakeDamage(Damage, EWeaponResistance::None, GetActorLocation());
			OnCharacterHitArrow(InflictedDamage);
		}
		else
		{
			OnEnvironmentHitArrow();
		}
		Destroy();
	}
}

float AArrow::GetInitialVelocity()
{
	return ProjectileMovementComponent->InitialSpeed;
}

void AArrow::SetVelocity(FVector NewVelocity)
{
	ProjectileMovementComponent->Velocity = NewVelocity;
}
