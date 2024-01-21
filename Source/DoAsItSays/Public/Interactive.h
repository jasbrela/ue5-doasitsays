// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactive.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractive : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DOASITSAYS_API IInteractive
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	virtual void OnEnterRange() = 0;
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	virtual void Interact() = 0;
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	virtual void OnExitRange() = 0;
};
