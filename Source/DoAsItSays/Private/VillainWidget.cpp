// Fill out your copyright notice in the Description page of Project Settings.


#include "VillainWidget.h"

#include "Components/TextBlock.h"

UVillainWidget::UVillainWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

void UVillainWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UVillainWidget::Hide()
{
	Subtitles->SetVisibility(ESlateVisibility::Collapsed);
	Timer->SetVisibility(ESlateVisibility::Collapsed);
}

void UVillainWidget::ShowTimer(int seconds)
{
	if (Timer)
	{
		if (Timer->GetVisibility() != ESlateVisibility::Visible)
		{
			Timer->SetVisibility(ESlateVisibility::Visible);
		}
		Timer->SetText(FText::AsNumber(seconds));
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("TIMER IS NULL"));
	}
}

void UVillainWidget::ShowSubtitles(FString text, int seconds)
{
	if (Subtitles)
	{
		if (Subtitles->GetVisibility() != ESlateVisibility::Visible)
		{
			Subtitles->SetVisibility(ESlateVisibility::Visible);
		}

		if (const UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(ClearSubtitlesTimerHandle);

			const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UVillainWidget::ClearSubtitles);
			World->GetTimerManager().SetTimer(ClearSubtitlesTimerHandle, TimerDelegate, seconds, false);
		}

		Subtitles->SetText(FText::FromString(text));
	}

}

void UVillainWidget::ClearSubtitles()
{
	if (Subtitles)
	{
		Subtitles->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UVillainWidget::ClearTimer()
{
	if (Timer)
	{
		Timer->SetVisibility(ESlateVisibility::Collapsed);
	}
}
