// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpObject.generated.h"

class UCapsuleComponent;

UCLASS()
class DOASITSAYS_API APickUpObject : public AActor
{
	GENERATED_BODY()
	
public:	
	APickUpObject();
	virtual void Tick(float DeltaTime) override;
	void OnPickUpObject();
	void OnDropObject();

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Collision;
};
