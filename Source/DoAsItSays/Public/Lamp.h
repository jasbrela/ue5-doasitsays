// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lamp.generated.h"

class USpotLightComponent;

UCLASS()
class DOASITSAYS_API ALamp : public AActor
{
	GENERATED_BODY()
	
public:	
	ALamp();
	virtual void Tick(float DeltaTime) override;
	
	void ToggleCircuit(bool On);
	void Toggle(bool On);
	void UpdateVisual() const;

protected:
	virtual void BeginPlay() override;

private:
	bool bIsOn;
	bool bIsCircuitOn;
	
	UPROPERTY(EditDefaultsOnly)
	USpotLightComponent* Light;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* EmissiveMaterial;
};
