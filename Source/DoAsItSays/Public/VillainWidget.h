// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VillainWidget.generated.h"

struct FSentenceData;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DOASITSAYS_API UVillainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UVillainWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void Hide();
	
	UFUNCTION(BlueprintCallable)
	void ShowTimer(int seconds);
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentDialogue(TArray<FSentenceData> sentences);

	UFUNCTION(BlueprintCallable)
	void ClearSubtitles();
	
	UFUNCTION(BlueprintCallable)
	void ClearTimer();

	TArray<FSentenceData> CurrentDialogue;
	int CurrentSentenceIndex;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CaptionText;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* CaptionFadeInAnim;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* CaptionFadeOutAnim;

private:
	FTimerHandle ShowCaptionTimerHandle;
	
	UFUNCTION(BlueprintCallable)
	void ShowNextCaption();
};
