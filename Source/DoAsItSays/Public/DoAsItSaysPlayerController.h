// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DoAsItSaysPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class DOASITSAYS_API ADoAsItSaysPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;
	
protected:
	virtual void BeginPlay() override;
};
