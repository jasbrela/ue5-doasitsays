// Fill out your copyright notice in the Description page of Project Settings.


#include "LookDeathZone.h"

#include "Components/BoxComponent.h"

ALookDeathZone::ALookDeathZone()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = Box;
}

void ALookDeathZone::BeginPlay()
{
	Super::BeginPlay();
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALookDeathZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALookDeathZone::OnMissionStatusChanged(int ID, bool Completed)
{
	if (AffectedAfterMissionCompletedID == ID)
	{
		Box->SetCollisionEnabled(Completed ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}
}

