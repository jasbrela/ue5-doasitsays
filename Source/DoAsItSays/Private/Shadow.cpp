#include "Shadow.h"

#include "Components/CapsuleComponent.h"

AShadow::AShadow()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	
	RootComponent = Mesh;
	Capsule->SetupAttachment(RootComponent);
}


void AShadow::OnMissionStatusChanged(int ID, bool Completed)
{
	if (ID == AffectedAfterMissionCompletedID)
	{
		Completed ? Enable() : Disable();
	}
}

void AShadow::SwitchExpression(UMaterialInterface* ExpressionMaterial)
{
	if (ExpressionMaterial)
	{
		Mesh->SetMaterial(0, ExpressionMaterial);
	}
}

void AShadow::Enable()
{
	Mesh->SetVisibility(true);
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AShadow::Disable()
{
	Mesh->SetVisibility(false);
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShadow::BeginPlay()
{
	Super::BeginPlay();
	Disable();
}

void AShadow::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

