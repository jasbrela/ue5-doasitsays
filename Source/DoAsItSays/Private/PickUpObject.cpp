// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpObject.h"

#include "Components/BoxComponent.h"

// Sets default values
APickUpObject::APickUpObject()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

	RootComponent = Box;
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APickUpObject::BeginPlay()
{
	Super::BeginPlay();
}

void APickUpObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickUpObject::OnPickUpObject()
{
	Mesh->SetMaterial(0, FixedMaterial);
	Box->SetSimulatePhysics(false);
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APickUpObject::OnDropObject()
{
	Mesh->SetMaterial(0, DefaultMaterial);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetSimulatePhysics(true);
}
