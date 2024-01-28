// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Components/BoxComponent.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;
		
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	
	RootComponent = Mesh;
	Box->SetupAttachment(RootComponent);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::OnMissionCompleted(int ID)
{
	if (ID == this->AffectedAfterMissionCompletedID)
	{
		Mesh->AddLocalRotation(FRotator(0, AddedLocalRollAfterMission, 0));
	}
}

