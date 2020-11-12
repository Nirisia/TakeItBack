// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"



#include "PlayerCharacter.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = TriggerBox;

	RespawnPointBillboard = CreateDefaultSubobject<UBillboardComponent>("RespawnPoint");
	RespawnPointBillboard->bIsScreenSizeScaled = true;
}

FVector ACheckpoint::GetRespawnLocation()
{
	return RespawnPointBillboard->GetComponentLocation();
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnBeginOverlap);
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckpoint::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{
		Player->Checkpoint = this;
	}
}

