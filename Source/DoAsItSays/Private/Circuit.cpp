// Fill out your copyright notice in the Description page of Project Settings.


#include "Circuit.h"

#include "Lamp.h"
#include "Villain.h"
#include "Components/LightComponent.h"
#include "Engine/Light.h"

ACircuit::ACircuit()
{
	CircuitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CircuitMesh"));
	CircuitDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CircuitDoorMesh"));
	
	RootComponent = CircuitMesh;
	CircuitDoorMesh->SetupAttachment(RootComponent);
	
	PrimaryActorTick.bCanEverTick = false;
	bIsInteractive = false;
	
	bIsOn = true;
}

void ACircuit::Interact(EInteractionEffect Effect)
{
	bIsOn = !bIsOn;

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
