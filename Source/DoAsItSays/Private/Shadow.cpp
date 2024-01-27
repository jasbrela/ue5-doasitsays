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


void AShadow::OnMissionCompleted(int ID)
{
}

void AShadow::SwitchExpression(UMaterialInterface* ExpressionMaterial)
{
	Mesh->SetMaterial(0, ExpressionMaterial);
}

void AShadow::BeginPlay() { Super::BeginPlay(); }
void AShadow::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

