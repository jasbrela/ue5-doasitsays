// Copyright Epic Games, Inc. All Rights Reserved.


#include "DoAsItSaysPlayerController.h"
#include "EnhancedInputSubsystems.h"

void ADoAsItSaysPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}