// Copyright Epic Games, Inc. All Rights Reserved.

#include "DDGameMode.h"
#include "DDCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADDGameMode::ADDGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
