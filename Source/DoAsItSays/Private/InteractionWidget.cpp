// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"


UInteractionWidget::UInteractionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

void UInteractionWidget::ToggleInteraction(bool bInteractiveActorFound, FString Instruction)
{
	if (bInteractiveActorFound)
	{
		Crosshair->SetOpacity(1);
		const FString TooltipText = TEXT("[E] " + Instruction);	
		Tooltip->SetText(FText::FromString(TooltipText));
		Tooltip->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		Crosshair->SetOpacity(0.4);
		Tooltip->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}