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
	
	Actors.Empty();
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UAffectedByMission::StaticClass(), Actors);
	
	for (AActor* Actor : Actors)
	{
		if (IAffectedByMission* Affected = Cast<IAffectedByMission>(Actor))
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

		if (TickingAudio)
		{
			if (!TickingAudio->IsPlaying())
			{
				TickingAudio->FadeIn(2);

				GetWorldTimerManager().ClearTimer(TickingTimerHandle);

				GetWorldTimerManager().SetTimer(TickingTimerHandle, TimerDelegate, 0, false,
				                                static_cast<float>(CurrentMission->DialogueDurationInSeconds + CurrentMission->TimeFrameInSeconds));
			}
		}

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
	GetWorldTimerManager().PauseTimer(TickingTimerHandle);

	TickingAudio->FadeOut(2, 0);

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
	
	UE_LOG(LogTemp, Warning, TEXT("Failed to set Villain's Expression"));
	return Mesh->GetMaterial(0);
}

void AVillain::OnDialogueFinished()
{
	bIsInteractive = true;
	ShadowWhispers->FadeOut(3, 0);
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
		UE_LOG(LogTemp, Warning, TEXT("Mark mission as completed: %d"), ID);
		
		UMaterialInterface* ExpressionMaterial = SwitchExpression(CurrentMission->ExpressionAfterCompletion);
		
		for (IAffectedByMission* Affected : AffectedByMissionActors)
		{
			if (Affected)
			{
				// Update Affected Actors
				Affected->OnMissionStatusChanged(CurrentMission->ID, true);
			}
		}

		// Switch Expression
		for (AShadow* Shadow : OtherShadows)
		{
			if (Shadow)
			{
				Shadow->SwitchExpression(ExpressionMaterial);
			}
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
		if (CurrentMission->ID == ID)
		{
			CurrentMission->bIsCompleted = false;

			if (CurrentMission->ID != 2)
			{
				bIsInteractive = false;
			}

			for (IAffectedByMission* Affected : AffectedByMissionActors)
			{
				if (Affected)
				{
					Affected->OnMissionStatusChanged(CurrentMission->ID, false);
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
