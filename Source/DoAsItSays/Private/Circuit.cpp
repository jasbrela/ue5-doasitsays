// Fill out your copyright notice in the Description page of Project Settings.


#include "Circuit.h"

#include "Lamp.h"
#include "Villain.h"
#include "Components/AudioComponent.h"

ACircuit::ACircuit()
{
	SwitchAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("SwitchSFX"));
	CircuitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CircuitMesh"));
	CircuitDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CircuitDoorMesh"));
	
	RootComponent = CircuitMesh;
	CircuitDoorMesh->SetupAttachment(RootComponent);
	SwitchAudio->SetupAttachment(RootComponent);
	
	PrimaryActorTick.bCanEverTick = false;
	bIsInteractive = false;
	bIsOn = true;
}

void ACircuit::Interact(EInteractionEffect Effect)
{
	bIsOn = !bIsOn;

	bIsOn ? Villain->MarkMissionAsUncompleted(2) : Villain->MarkMissionAsCompleted(2, false);

	SwitchAudio->Play();

	if (Lamps.Num() > 0)
	{
		for (int i = 0; i < Lamps.Num(); i++)
		{
			if (Lamps[i])
			{
				Lamps[i]->ToggleCircuit(bIsOn);
			}
		}
	}
	
}

void ACircuit::OnMissionStatusChanged(int ID, bool Completed)
{
	if (AffectedAfterMissionCompletedID == ID)
	{
		bIsInteractive = !Completed;
	}
}

void ACircuit::BeginPlay()
{
	Super::BeginPlay();
}

void ACircuit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACircuit::Enable()
{
	bIsInteractive = true;
}

void ACircuit::OnExitRange() { }
void ACircuit::OnEnterRange() { }
