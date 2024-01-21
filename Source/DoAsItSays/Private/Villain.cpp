// Fill out your copyright notice in the Description page of Project Settings.


#include "Villain.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AVillain::AVillain()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	Capsule->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AVillain::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVillain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVillain::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacted with %s"), *GetName());
}

void AVillain::OnExitRange()
{
	UE_LOG(LogTemp, Warning, TEXT("Entered Range of %s"), *GetName());
}

void AVillain::OnEnterRange()
{
	UE_LOG(LogTemp, Warning, TEXT("Exited Range of %s"), *GetName());
}

