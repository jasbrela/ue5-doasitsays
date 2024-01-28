// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	OpenAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("OpenAudio"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	
	RootComponent = Mesh;
	Box->SetupAttachment(RootComponent);
	OpenAudio->SetupAttachment(RootComponent);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADoor::OnMissionStatusChanged(int ID, bool Completed)
{

	if (ID == this->AffectedAfterMissionCompletedID)
	{
		if (AddedLocalRollAfterMission == 0) return;
		
		if (Completed)
		{
			OpenAudio->Play();
			Mesh->AddLocalRotation(FRotator(0, AddedLocalRollAfterMission, 0));
		}
	}
}

