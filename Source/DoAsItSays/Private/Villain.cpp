// Fill out your copyright notice in the Description page of Project Settings.


#include "Villain.h"

#include "Components/CapsuleComponent.h"
#include "MissionData.h"
#include "VillainWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AVillain::AVillain()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	Capsule->SetupAttachment(RootComponent);
}

void AVillain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVillain::BeginPlay()
{
	Super::BeginPlay();

	if (VillainWidgetClass)
	{
		if (const UWorld* World = GetWorld())
		{
			VillainWidget = CreateWidget<UVillainWidget>(UGameplayStatics::GetPlayerController(World, 0), VillainWidgetClass, TEXT("VillainUI"));
			VillainWidget->AddToViewport(0);
		}
	} else {
		UE_LOG(LogTemp, Error, TEXT("Missing VillainWidgetClass in %s"), *GetName());
	}
}

void AVillain::Interact()
{
	if (VillainWidget)
	{
		bIsActive = false;
		const FMissionData CurrentMission = Missions[CurrentMissionIndex];
		CurrentTimerSeconds = CurrentMission.TimeFrameInSeconds;
		
		if (CurrentTimerSeconds > 0)
		{
			if (const UWorld* World = GetWorld())
			{
				World->GetTimerManager().ClearTimer(TickingTimerHandle);
				const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &AVillain::UpdateTimerUI);
				
				VillainWidget->ShowTimer(CurrentTimerSeconds);
				
				World->GetTimerManager().SetTimer(TickingTimerHandle, TimerDelegate, 1.0f, true);
			}
		}

		if (CurrentMission.Instruction.Len() > 0)
		{
			VillainWidget->ShowSubtitles(CurrentMission.Instruction, 5);
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("What"));
	}
}

void AVillain::UpdateTimerUI()
{
	CurrentTimerSeconds--;
	VillainWidget->ShowTimer(CurrentTimerSeconds);

	if (CurrentTimerSeconds <= 0)
	{
		VillainWidget->Hide();
		JumpScare();
	}
}

void AVillain::JumpScare()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over"));
}

void AVillain::OnExitRange() { }
void AVillain::OnEnterRange() { }

