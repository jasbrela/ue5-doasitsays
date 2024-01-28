// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AffectedByMission.h"
#include "GameFramework/Actor.h"
#include "CompleteMissionArea.generated.h"

class AVillain;
class UBoxComponent;

UCLASS()
class DOASITSAYS_API ACompleteMissionArea : public AActor, public IAffectedByMission
{
	GENERATED_BODY()
	
public:	
	ACompleteMissionArea();
	virtual void Tick(float DeltaTime) override;
    virtual void OnMissionStatusChanged(int ID, bool Completed = true) override;

protected:
	virtual void BeginPlay() override;

private:	
    UPROPERTY(EditAnywhere)
    int AffectedAfterMissionCompletedID = 0;

	UPROPERTY(EditAnywhere)
    int CompleteMissionID = 0;
    
    UPROPERTY(EditDefaultsOnly)
    UBoxComponent* Box;

	UPROPERTY(EditInstanceOnly)
	AVillain* Villain;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
