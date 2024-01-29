#pragma once

#include "CoreMinimal.h"
#include "FSentenceData.h"
#include "InteractionEffect.h"
#include "VillainExpression.h"
#include "MissionData.generated.h"

USTRUCT(BlueprintType)
struct FMissionData : public FTableRowBase
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ID = 0;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EInteractionEffect> RequiredEffect = EInteractionEffect::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EVillainExpression> ExpressionAfterCompletion = EVillainExpression::Eyes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DialogueDurationInSeconds = 25;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TimeFrameInSeconds = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSentenceData> Sentences;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsCompleted = false;
};
