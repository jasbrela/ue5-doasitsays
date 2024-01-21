#pragma once

#include "CoreMinimal.h"
#include "MissionType.h"
#include "MissionData.generated.h"

USTRUCT(BlueprintType)
struct FMissionData : public FTableRowBase
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Instruction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMissionType> Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeFrameInSeconds;
};
