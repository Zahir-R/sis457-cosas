// Copyright Epic Games, Inc. All Rights Reserved.

#include "NavesUSFX_012026GameMode.h"
#include "NavesUSFX_012026Pawn.h"

ANavesUSFX_012026GameMode::ANavesUSFX_012026GameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ANavesUSFX_012026Pawn::StaticClass();
}

