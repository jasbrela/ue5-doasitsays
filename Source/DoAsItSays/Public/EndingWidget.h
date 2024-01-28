// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndingWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DOASITSAYS_API UEndingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UEndingWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Title;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MenuButtonText;

	UPROPERTY(meta = (BindWidget))
	UButton* MenuButton;

private:
	UFUNCTION()
	void OnClickMenuButton();

};
