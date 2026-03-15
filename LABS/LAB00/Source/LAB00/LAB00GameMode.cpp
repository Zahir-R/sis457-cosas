// Copyright Epic Games, Inc. All Rights Reserved.

#include "LAB00GameMode.h"
#include "LAB00Pawn.h"
#include "Enemy.h"
#include "Engine/World.h"

ALAB00GameMode::ALAB00GameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ALAB00Pawn::StaticClass();
}

void ALAB00GameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		for (int32 i = 0; i < 3; i++)
		{
			FVector SpawnLocation = FVector(i * 300.0f, 500.0f, 150.0f);
			FRotator SpawnRotator = FRotator::ZeroRotator;

			World->SpawnActor<AEnemy>(AEnemy::StaticClass(), SpawnLocation, SpawnRotator);
		}
	}
}