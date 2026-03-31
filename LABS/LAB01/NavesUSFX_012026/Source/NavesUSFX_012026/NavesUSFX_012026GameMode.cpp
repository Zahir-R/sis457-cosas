// Copyright Epic Games, Inc. All Rights Reserved.

#include "NavesUSFX_012026GameMode.h"
#include "NavesUSFX_012026Pawn.h"
#include "Enemigo.h"

ANavesUSFX_012026GameMode::ANavesUSFX_012026GameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ANavesUSFX_012026Pawn::StaticClass();
}

void ANavesUSFX_012026GameMode::BeginPlay()
{
	Super::BeginPlay();
	
	for (int32 i = 0; i < 10; i++)
	{
		FVector SpawnLocation = FVector(FMath::RandRange(-1000.0f, 1000.0f), FMath::RandRange(-1000.0f, 1000.0f), 200.0f);
		FRotator SpawnRotation = FRotator::ZeroRotator;
		GetWorld()->SpawnActor<AEnemigo>(AEnemigo::StaticClass(), SpawnLocation, SpawnRotation);
	}
}