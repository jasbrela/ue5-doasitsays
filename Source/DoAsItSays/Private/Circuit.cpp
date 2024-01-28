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

	SwitchAudio->Play();

	if (Lamps.Num() > 0)
	{
		for (ALamp* Lamp : Lamps)
		{
			if (IsValid(Lamp))
			{
				Lamp->ToggleCircuit(bIsOn);
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
