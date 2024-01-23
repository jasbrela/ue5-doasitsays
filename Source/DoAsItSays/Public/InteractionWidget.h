// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class DOASITSAYS_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UInteractionWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void ToggleTooltip(bool bInteractiveActorFound, FString Instruction = TEXT("Interact"));
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Tooltip;
	UPROPERTY(meta = (BindWidget))
	UImage* Crosshair;
};
