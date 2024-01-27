// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive.h"
#include "GameFramework/Actor.h"
#include "Circuit.generated.h"

class ALamp;
class ALight;
class AVillain;

UCLASS()
class DOASITSAYS_API ACircuit : public AActor, public IInteractive
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACircuit();
	virtual void Interact(EInteractionEffect Effect = EInteractionEffect::None) override;
	virtual void OnExitRange() override;
	virtual void OnEnterRange() override;
	virtual void Tick(float DeltaTime) override;
	void Enable();
	
protected:
	virtual void BeginPlay() override;

private:
	bool bIsOn;
	
	UPROPERTY(EditInstanceOnly)
	TArray<ALamp*> Lamps;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CircuitMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CircuitDoorMesh;
};
