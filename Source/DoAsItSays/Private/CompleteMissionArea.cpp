// Fill out your copyright notice in the Description page of Project Settings.


#include "CompleteMissionArea.h"

#include "DoAsItSaysCharacter.h"
#include "Villain.h"
#include "Components/BoxComponent.h"

ACompleteMissionArea::ACompleteMissionArea()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    RootComponent = Box;
}

void ACompleteMissionArea::BeginPlay()
{
	Super::BeginPlay();
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACompleteMissionArea::OnBoxBeginOverlap);
}

void ACompleteMissionArea::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ADoAsItSaysCharacter>(OtherActor))
	{
		Villain->MarkMissionAsCompleted(CompleteMissionID, true);
	}
}

void ACompleteMissionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACompleteMissionArea::OnMissionStatusChanged(int ID, bool Completed)
{
	if (AffectedAfterMissionCompletedID == ID)
	{
		Box->SetCollisionEnabled(Completed ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}
}

