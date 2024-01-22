// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VillainWidget.generated.h"

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
	void ShowSubtitles(FString text, int seconds);

	UFUNCTION(BlueprintCallable)
	void ClearSubtitles();
	
	UFUNCTION(BlueprintCallable)
	void ClearTimer();
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Timer;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Subtitles;

private:
	FTimerHandle ClearSubtitlesTimerHandle;
};
