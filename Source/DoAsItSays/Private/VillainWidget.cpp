// Fill out your copyright notice in the Description page of Project Settings.


#include "VillainWidget.h"

#include "FSentenceData.h"
#include "Components/TextBlock.h"

UVillainWidget::UVillainWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

void UVillainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TimerDelegate = FTimerDelegate::CreateUObject(this, &UVillainWidget::ShowNextCaption);
}

void UVillainWidget::Hide()
{
	CaptionText->SetVisibility(ESlateVisibility::Collapsed);
	TimerText->SetVisibility(ESlateVisibility::Collapsed);
}

void UVillainWidget::ShowTimer(int Seconds)
{
	if (TimerText)
	{
		if (TimerText->GetVisibility() != ESlateVisibility::Visible)
		{
			TimerText->SetVisibility(ESlateVisibility::Visible);
		}
		TimerText->SetText(FText::AsNumber(Seconds));
	}
}

void UVillainWidget::SetCurrentDialogue(const TArray<FSentenceData>& Sentences)
{
	CurrentDialogue = Sentences;
	CurrentSentenceIndex = 0;
	ShowNextCaption();
}

void UVillainWidget::ShowNextCaption()
{
	if (CurrentSentenceIndex >= CurrentDialogue.Num())
	{
		ClearSubtitles();
		return;
	}
	
	if (CaptionText)
	{
		const FSentenceData CurrentSentence = CurrentDialogue[CurrentSentenceIndex];

		if (CurrentSentence.Text == TEXT("N/A"))
		{
			PlayAnimation(CaptionFadeOutAnim);
		} else
		{
			CaptionText->SetText(FText::FromString(CurrentSentence.Text));
			
			PlayAnimation(CaptionFadeInAnim);
		}

		if (const UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(ShowCaptionTimerHandle);
			World->GetTimerManager().SetTimer(ShowCaptionTimerHandle, TimerDelegate,  CurrentSentence.Seconds, false);
		}
		
		CurrentSentenceIndex++;
	}
}

void UVillainWidget::ClearSubtitles()
{
	if (CaptionText)
	{
		PlayAnimation(CaptionFadeOutAnim);
	}
}

void UVillainWidget::ClearTimer()
{
	if (TimerText)
	{
		TimerText->SetVisibility(ESlateVisibility::Collapsed);
	}
}
