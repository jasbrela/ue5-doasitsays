// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MissionData.generated.h"

USTRUCT(BlueprintType)
struct DOASITSAYS_API FMissionData
{
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Instruction;
	
	FMissionData();	
	~FMissionData();
};
