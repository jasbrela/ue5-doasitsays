// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionEffect.h"
#include "UObject/Interface.h"
#include "Interactive.generated.h"

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
	virtual void Interact(EInteractionEffect Effect = EInteractionEffect::None) = 0;
	virtual void OnExitRange() = 0;
private:
};
