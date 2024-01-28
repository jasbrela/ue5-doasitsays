// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AffectedByMission.h"
#include "GameFramework/Actor.h"
#include "LookDeathZone.generated.h"

class UBoxComponent;

UCLASS()
class DOASITSAYS_API ALookDeathZone : public AActor, public IAffectedByMission
{
	GENERATED_BODY()
	
public:	
	ALookDeathZone();
	virtual void Tick(float DeltaTime) override;
	virtual void OnMissionStatusChanged(int ID, bool Completed = true) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	int AffectedAfterMissionCompletedID = 0;
    
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Box;

};
