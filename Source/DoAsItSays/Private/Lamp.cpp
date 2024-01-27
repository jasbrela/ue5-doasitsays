// Fill out your copyright notice in the Description page of Project Settings.


#include "Lamp.h"

#include "Components/SpotLightComponent.h"

ALamp::ALamp()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	
	RootComponent = Mesh;
	Light->SetupAttachment(RootComponent);
}

void ALamp::BeginPlay()
{
	Super::BeginPlay();
}

void ALamp::ToggleCircuit(const bool On)
{
	bIsCircuitOn = On;
	
	UpdateVisual();
}

void ALamp::Toggle(bool On)
{
	bIsOn = On;
	
	if (bIsCircuitOn)
	{
		UpdateVisual();
	}
}

void ALamp::UpdateVisual() const
{
	Mesh->SetMaterial(0, bIsOn && bIsCircuitOn ? EmissiveMaterial : DefaultMaterial);
	Light->SetVisibility(bIsOn && bIsCircuitOn);
}

void ALamp::Tick(float DeltaTime) {	Super::Tick(DeltaTime); }
