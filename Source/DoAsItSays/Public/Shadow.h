// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shadow.generated.h"

class UCapsuleComponent;
enum EVillainExpression : int;

UCLASS()
class DOASITSAYS_API AShadow : public AActor
{
	GENERATED_BODY()
	
public:	
	AShadow();
	virtual void Tick(float DeltaTime) override;
	void OnMissionCompleted(int ID);
	void SwitchExpression(UMaterialInterface* ExpressionMaterial);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditInstanceOnly)
	int ShowAfterMissionCompletedID = 0;

};