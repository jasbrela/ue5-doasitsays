// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AffectedByMission.h"
#include "Interactive.h"
#include "GameFramework/Actor.h"
#include "Circuit.generated.h"

class ALamp;
class ALight;
class AVillain;

UCLASS()
class DOASITSAYS_API ACircuit : public AActor, public IInteractive, public IAffectedByMission
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACircuit();
	virtual void Interact(EInteractionEffect Effect = EInteractionEffect::None) override;
	virtual void OnMissionStatusChanged(int ID, bool Completed = true) override;
	virtual void OnExitRange() override;
	virtual void OnEnterRange() override;
	virtual void Tick(float DeltaTime) override;
	void Enable();
	bool bIsOn;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditInstanceOnly)
	AVillain* Villain;
	
	UPROPERTY(EditAnywhere)
	int AffectedAfterMissionCompletedID = 0;

	UPROPERTY(EditDefaultsOnly, Category= "Audio")
	UAudioComponent* SwitchAudio;
	
	UPROPERTY(EditInstanceOnly)
	TArray<ALamp*> Lamps;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CircuitMesh;
};
