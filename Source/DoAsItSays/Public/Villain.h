// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionEffect.h"
#include "Interactive.h"
#include "GameFramework/Actor.h"
#include "MissionData.h"
#include "Villain.generated.h"

class UVillainWidget;
class UCapsuleComponent;

UCLASS()
class DOASITSAYS_API AVillain : public AActor, public IInteractive
{
	GENERATED_BODY()
	
public:	
	AVillain();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(EInteractionEffect Effect = EInteractionEffect::None) override;
	virtual void OnExitRange() override;
	virtual void OnEnterRange() override;
	void MarkMissionAsCompleted(int id);
	void MarkMissionAsUncompleted(int id);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int CurrentMissionIndex = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> VillainWidgetClass;
	
	UPROPERTY()
	UVillainWidget* VillainWidget;

	
private:
	UPROPERTY(VisibleInstanceOnly)
	TArray<FMissionData> Missions;

	UPROPERTY(EditDefaultsOnly)
	UDataTable* Table;
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditDefaultsOnly)
	UAudioComponent* TickingAudio;

	UPROPERTY(EditDefaultsOnly)
	UAudioComponent* ShadowWhispers;
	
	void GiveMission();
	void NextMission();
	void JumpScare();
	void OnDialogueFinished();
	void UpdateTimerUI();
	void GiveInitialMission();
	int CurrentTimerSeconds = 0;
	FMissionData CurrentMission;
	FTimerHandle TickingTimerHandle;
	FTimerHandle ShadowWhispersTimerHandle;
	FTimerDelegate UpdateTimerUIDelegate;
	FTimerDelegate StopWhispersDelegate;
};
