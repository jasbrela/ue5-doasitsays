// Fill out your copyright notice in the Description page of Project Settings.


#include "Villain.h"

#include "Components/CapsuleComponent.h"
#include "MissionData.h"
#include "VillainWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AVillain::AVillain()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	TickingAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("TickingAudio"));
	ShadowWhispers = CreateDefaultSubobject<UAudioComponent>(TEXT("WhispersAudio"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));

	RootComponent = Mesh;
	Capsule->SetupAttachment(RootComponent);
	TickingAudio->SetupAttachment(RootComponent);
	ShadowWhispers->SetupAttachment(RootComponent);
}

void AVillain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVillain::BeginPlay()
{
	Super::BeginPlay();
	const TArray<FName> Names = Table->GetRowNames();
	const FString Context = TEXT("");
	
	for (int i = 0; i < Names.Num(); i++)
	{
		const FMissionData* Mission = Table->FindRow<FMissionData>(Names[i], Context);
		Missions.Add(*Mission);
	}
	
	ShadowWhispers->bAlwaysPlay = true;
	UpdateTimerUIDelegate = FTimerDelegate::CreateUObject(this, &AVillain::UpdateTimerUI);
	StopWhispersDelegate = FTimerDelegate::CreateUObject(this, &AVillain::OnDialogueFinished);

	Tooltip = TEXT("Answer");

	if (VillainWidgetClass)
	{
		if (const UWorld* World = GetWorld())
		{
			VillainWidget = CreateWidget<UVillainWidget>(UGameplayStatics::GetPlayerController(World, 0),
			                                             VillainWidgetClass, TEXT("VillainUI"));
			VillainWidget->AddToViewport(0);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Missing VillainWidgetClass in %s"), *GetName());
	}

	GiveInitialMission();
}

void AVillain::Interact(EInteractionEffect Effect)
{
	if (VillainWidget)
	{
		if (CurrentMission.RequiredEffect == Effect || CurrentMission.RequiredEffect == EInteractionEffect::None)
		{
			if (CurrentMission.bIsCompleted)
			{
				if (CurrentMission.RequiredEffect == EInteractionEffect::None)
				{
					NextMission();
				}
			} else
			{
				if (CurrentMission.RequiredEffect != EInteractionEffect::None)
				{
					CurrentMission.bIsCompleted = true;
					NextMission();
				}
			}
			// TODO: "Deliver" is showing even if the player is not carrying an object.
			Tooltip = CurrentMission.RequiredEffect == EInteractionEffect::None ? Tooltip = TEXT("Answer") : TEXT("Deliver");
		}
	}
}

void AVillain::UpdateTimerUI()
{
	if (ShadowWhispers->IsPlaying())
	{
		OnDialogueFinished();
	}

	if (CurrentTimerSeconds <= 0)
	{
		VillainWidget->Hide();
		JumpScare();
		TickingAudio->Stop();
	}

	VillainWidget->ShowTimer(CurrentTimerSeconds);

	CurrentTimerSeconds--;
}

void AVillain::GiveInitialMission()
{
	GiveMission();
	MarkMissionAsCompleted(0);
	bIsInteractive = false;
}

void AVillain::GiveMission()
{
	ShadowWhispers->FadeIn(3);
	CurrentMission = Missions[CurrentMissionIndex];

	MarkMissionAsUncompleted(CurrentMission.ID);
	CurrentTimerSeconds = CurrentMission.TimeFrameInSeconds;

	if (CurrentTimerSeconds > 0)
	{
		TickingAudio->FadeIn(2);

		GetWorldTimerManager().ClearTimer(TickingTimerHandle);

		GetWorldTimerManager().SetTimer(TickingTimerHandle, UpdateTimerUIDelegate, 1.0f, true,
		                                static_cast<float>(CurrentMission.DialogueDurationInSeconds));
	}

	if (CurrentMission.DialogueDurationInSeconds > 0)
	{
		GetWorldTimerManager().ClearTimer(ShadowWhispersTimerHandle);

		GetWorldTimerManager().SetTimer(ShadowWhispersTimerHandle, StopWhispersDelegate,
		                                static_cast<float>(CurrentMission.DialogueDurationInSeconds), false);
	}

	if (CurrentMission.Sentences.Num() > 0)
	{
		VillainWidget->SetCurrentDialogue(CurrentMission.Sentences);
	}
}

void AVillain::NextMission()
{
	GetWorldTimerManager().PauseTimer(TickingTimerHandle);

	TickingAudio->FadeOut(2, 0);

	CurrentMissionIndex++;
	GiveMission();
}

void AVillain::JumpScare()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over"));
}

void AVillain::OnDialogueFinished()
{
	bIsInteractive = true;
	ShadowWhispers->FadeOut(3, 0);
}

void AVillain::OnExitRange()
{
}

void AVillain::OnEnterRange()
{
}

void AVillain::MarkMissionAsCompleted(int id)
{
	if (CurrentMission.ID == id)
	{
		CurrentMission.bIsCompleted = true;
		bIsInteractive = true;
	}
}

void AVillain::MarkMissionAsUncompleted(int id)
{
	if (CurrentMission.ID == id)
	{
		CurrentMission.bIsCompleted = false;
		bIsInteractive = false;
	}
}
