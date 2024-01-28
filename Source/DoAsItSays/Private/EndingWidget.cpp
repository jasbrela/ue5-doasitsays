// Fill out your copyright notice in the Description page of Project Settings.


#include "EndingWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

UEndingWidget::UEndingWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

void UEndingWidget::NativeConstruct()
{
	MenuButton->OnClicked.AddDynamic(this, &UEndingWidget::OnClickMenuButton);

	Super::NativeConstruct();
}

void UEndingWidget::OnClickMenuButton()
{
	UGameplayStatics::OpenLevel(this, TEXT("Game"));
}
