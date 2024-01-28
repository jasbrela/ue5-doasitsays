// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AffectedByMission.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UBoxComponent;

UCLASS()
class DOASITSAYS_API ADoor : public AActor, public IAffectedByMission
{
	GENERATED_BODY()
	
public:	
	ADoor();
	virtual void Tick(float DeltaTime) override;
	virtual void OnMissionStatusChanged(int ID, bool Completed = true) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
	int AffectedAfterMissionCompletedID = 0;
	
	UPROPERTY(EditAnywhere)
	int AddedLocalRollAfterMission = 0;
};
