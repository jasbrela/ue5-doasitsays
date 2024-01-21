// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactive.h"
#include "GameFramework/Actor.h"
#include "Villain.generated.h"

class UCapsuleComponent;

UCLASS()
class DOASITSAYS_API AVillain : public AActor, public IInteractive
{
	GENERATED_BODY()
	
public:	
	AVillain();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact() override;
	virtual void OnExitRange() override;
	virtual void OnEnterRange() override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* Capsule;
};
