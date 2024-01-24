#pragma once

#include "CoreMinimal.h"
#include "MissionType.h"
#include "FSentenceData.h"
#include "InteractionEffect.h"
#include "MissionData.generated.h"

USTRUCT(BlueprintType)
struct FMissionData : public FTableRowBase
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMissionType> Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EInteractionEffect> RequiredEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DialogueDurationInSeconds;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeFrameInSeconds;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSentenceData> Sentences;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsCompleted;
};
