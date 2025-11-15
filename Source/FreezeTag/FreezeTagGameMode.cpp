// Copyright Epic Games, Inc. All Rights Reserved.

#include "FreezeTagGameMode.h"
#include "FreezeTagCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFreezeTagGameMode::AFreezeTagGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
