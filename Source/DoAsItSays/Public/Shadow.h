// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AffectedByMission.h"
#include "GameFramework/Actor.h"
#include "Shadow.generated.h"

struct FMissionData;
class UCapsuleComponent;
enum EVillainExpression : int;

UCLASS()
class DOASITSAYS_API AShadow : public AActor, public IAffectedByMission
{
	GENERATED_BODY()
	
public:	
	AShadow();
	virtual void Tick(float DeltaTime) override;
	virtual void OnMissionCompleted(int ID) override;
	void SwitchExpression(UMaterialInterface* ExpressionMaterial);
	void Enable();
	void Disable();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* Capsule;
	
	UPROPERTY(EditAnywhere)
	int AffectedAfterMissionCompletedID = 0;
};
