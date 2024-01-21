// Copyright Epic Games, Inc. All Rights Reserved.

#include "DoAsItSaysGameMode.h"
#include "DoAsItSaysCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADoAsItSaysGameMode::ADoAsItSaysGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
