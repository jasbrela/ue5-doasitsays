// Fill out your copyright notice in the Description page of Project Settings.


#include "Villain.h"

#include "Circuit.h"
#include "MissionData.h"
#include "Shadow.h"
#include "VillainWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AVillain::AVillain()
{
	PrimaryActorTick.bCanEverTick = false;

	TickingAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("TickingAudio"));
	ShadowWhispers = CreateDefaultSubobject<UAudioComponent>(TEXT("WhispersAudio"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	
	RootComponent = Mesh;
	Capsule->SetupAttachment(RootComponent);
	TickingAudio->SetupAttachment(RootComponent);
	ShadowWhispers->SetupAttachment(RootComponent);
	
}

void AVillain::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void AVillain::BeginPlay()
{
	const TArray<FName> Names = Table->GetRowNames();
	const FString Context = TEXT("");
	
	for (int i = 0; i < Names.Num(); i++)
	{
		FMissionData* Mission = Table->FindRow<FMissionData>(Names[i], Context);
		Missions.Add(Mission);
	}
	
	ShadowWhispers->bAlwaysPlay = true;
	TimerDelegate = FTimerDelegate::CreateUObject(this, &AVillain::OnTimerEnd);
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

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UAffectedByMission::StaticClass(), Actors);

	for (int i = 0; i < Actors.Num(); i++)
	{
		if (IAffectedByMission* Affected = Cast<IAffectedByMission>(Actors[i]))
		{
			AffectedByMissionActors.Add(Affected);
		}
	}
}

void AVillain::Interact(EInteractionEffect Effect)
{
	if (VillainWidget)
	{
		if (CurrentMission)
		{
			if (CurrentMission->RequiredEffect == Effect || CurrentMission->RequiredEffect == EInteractionEffect::None)
			{
				// Mission "Power Source"
				if (CurrentMission->ID == 2)
				{
					Circuit->bIsOn ? MarkMissionAsUncompleted(CurrentMission->ID) : MarkMissionAsCompleted(CurrentMission->ID, false);
				}
				
				if (CurrentMission->bIsCompleted)
				{
					if (CurrentMission->RequiredEffect == EInteractionEffect::None)
					{
						NextMission();
					}
				}
				else
				{
					if (CurrentMission->RequiredEffect != EInteractionEffect::None)
					{
						MarkMissionAsCompleted(CurrentMission->ID, true);
					}
				}
				//Tooltip = CurrentMission->RequiredEffect == EInteractionEffect::None ? Tooltip = TEXT("Answer") : TEXT("Deliver");
			}
		}
	}
}

void AVillain::OnTimerEnd()
{
	if (VillainWidget)
	{
		VillainWidget->Hide();
	}
	
	JumpScare();

	if (TickingAudio)
	{
		TickingAudio->Stop();
	}

	//VillainWidget->ShowTimer(CurrentTimerSeconds);
}

void AVillain::GiveInitialMission()
{
	GiveMission();
	bIsInteractive = false;
}

void AVillain::GiveMission()
{
	if (ShadowWhispers)
	{
		ShadowWhispers->FadeIn(3);
	}

	if (CurrentMissionIndex >= Missions.Num())
	{
		UGameplayStatics::OpenLevel(this, TEXT("GoodEnding_Level"));
		return;
	}
	
	CurrentMission = Missions[CurrentMissionIndex];
	
	UE_LOG(LogTemp, Warning, TEXT("Started Mission: %d"), CurrentMission->ID);

	MarkMissionAsUncompleted(CurrentMission->ID);

	if (CurrentMission)
	{
		if (CurrentMission->ID == 2) // Hardcoded :(
		{
			Circuit->Enable();
		}

		if (CurrentMission->TimeFrameInSeconds > 0)
		{
			if (TickingAudio)
			{
				if (!TickingAudio->IsPlaying())
				{
					TickingAudio->FadeIn(2);

				}
			}
		}
		
		GetWorldTimerManager().ClearTimer(GameOverTimerHandle);
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, TimerDelegate, 0, false,
			                            static_cast<float>(CurrentMission->DialogueDurationInSeconds + CurrentMission->TimeFrameInSeconds));

		if (CurrentMission->DialogueDurationInSeconds > 0)
		{
			GetWorldTimerManager().ClearTimer(ShadowWhispersTimerHandle);

			GetWorldTimerManager().SetTimer(ShadowWhispersTimerHandle, StopWhispersDelegate,
			                                static_cast<float>(CurrentMission->DialogueDurationInSeconds), false);
		}

		if (VillainWidget)
		{
			if (CurrentMission->Sentences.Num() > 0)
			{
				VillainWidget->SetCurrentDialogue(CurrentMission->Sentences);
			}
		}
	}
}

void AVillain::NextMission()
{
	if (TickingAudio) {
		TickingAudio->FadeOut(2, 0);
	}

	CurrentMissionIndex++;
	GiveMission();
}

void AVillain::JumpScare()
{
	// TODO: Game Over
	UGameplayStatics::OpenLevel(this, TEXT("BadEnding_Level"));
}

UMaterialInterface* AVillain::SwitchExpression(EVillainExpression Expression)
{
	if (ExpressionMaterials.Num() > Expression)
	{
		if (ExpressionMaterials[Expression])
		{
			Mesh->SetMaterial(0, ExpressionMaterials[Expression]);
			return ExpressionMaterials[Expression];
		}
	}
	
	return Mesh->GetMaterial(0);
}

void AVillain::OnDialogueFinished()
{
	bIsInteractive = true;
	if (ShadowWhispers)
	{
		ShadowWhispers->FadeOut(3, 0);
	}
}

void AVillain::OnExitRange() { }

void AVillain::OnEnterRange()
{

	if (CurrentMission)
	{
		if (CurrentMission->ID == 0)
		{
			MarkMissionAsCompleted(0, true);
		}
	}
}

void AVillain::MarkMissionAsCompleted(const int ID, const bool ForceNextMission)
{
	if (CurrentMission)
	{
		if (CurrentMission->bIsCompleted) return;
		
		UMaterialInterface* ExpressionMaterial = SwitchExpression(CurrentMission->ExpressionAfterCompletion);
		
		for (int i = 0; i < AffectedByMissionActors.Num(); i++)
		{
			if (AffectedByMissionActors[i])
			{
				// Update Affected Actors
				AffectedByMissionActors[i]->OnMissionStatusChanged(CurrentMission->ID, true);
			}
		}

		// Switch Expression
		if (ExpressionMaterial)
		{
			for (int i = 0; i < OtherShadows.Num(); i++)
			{
				if (OtherShadows[i])
				{
					OtherShadows[i]->SwitchExpression(ExpressionMaterial);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Expression NULL"));
		}
		
		// Finally mark mission as completed
		if (CurrentMission->ID == ID)
		{
			CurrentMission->bIsCompleted = true;
			bIsInteractive = true;

			if (ForceNextMission)
			{
				NextMission();
			}
		}
	}

}

void AVillain::MarkMissionAsUncompleted(const int ID)
{
	if (CurrentMission)
	{
		if (!CurrentMission->bIsCompleted) return;
		
		if (CurrentMission->ID == ID)
		{
			CurrentMission->bIsCompleted = false;

			if (CurrentMission->ID != 2)
			{
				bIsInteractive = false;
			}

			for (int i = 0; i < AffectedByMissionActors.Num(); i++)
			{
				if (AffectedByMissionActors[i])
				{
					AffectedByMissionActors[i]->OnMissionStatusChanged(CurrentMission->ID, false);
				}
			}
		}
	}
}

EInteractionEffect AVillain::GetRequiredEffect() const
{
	if (CurrentMission)
	{
		return CurrentMission->RequiredEffect;
	}
	return EInteractionEffect::None;
}
