// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpObject.h"

#include "Components/CapsuleComponent.h"

// Sets default values
APickUpObject::APickUpObject()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));

	RootComponent = Collision;
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
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collision->SetSimulatePhysics(false);
}

void APickUpObject::OnDropObject()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetSimulatePhysics(true);
}
