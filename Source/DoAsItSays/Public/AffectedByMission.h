// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AffectedByMission.generated.h"

struct FMissionData;

UINTERFACE(MinimalAPI)
class UAffectedByMission : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DOASITSAYS_API IAffectedByMission
{
	GENERATED_BODY()

public:
	virtual void OnMissionStatusChanged(int ID, bool Completed = true);
};
