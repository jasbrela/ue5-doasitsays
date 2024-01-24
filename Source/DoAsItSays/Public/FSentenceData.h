#pragma once

#include "CoreMinimal.h"
#include "FSentenceData.generated.h"

enum EVillainExpression : int;

USTRUCT(BlueprintType)
struct FSentenceData
{
	GENERATED_BODY();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text = TEXT("N/A");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EVillainExpression> Expression;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Seconds;
};
