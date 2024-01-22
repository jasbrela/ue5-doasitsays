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

public:
	bool bIsInteractive = true;
	FString Tooltip = "Interact";
	virtual void OnEnterRange() = 0;
	virtual void Interact() = 0;
	virtual void OnExitRange() = 0;
private:
};
