// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpObject.generated.h"

enum EInteractionEffect : int;
class UBoxComponent;

UCLASS()
class DOASITSAYS_API APickUpObject : public AActor
{
	GENERATED_BODY()
	
public:	
	APickUpObject();
	virtual void Tick(float DeltaTime) override;
	void OnPickUpObject();
	void OnDropObject();
	
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EInteractionEffect> Effect;

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Box;
	
	UPROPERTY(EditDefaultsOnly, Category="PickUp")
	UMaterialInterface* DefaultMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category="PickUp")
	UMaterialInterface* FixedMaterial;
};
