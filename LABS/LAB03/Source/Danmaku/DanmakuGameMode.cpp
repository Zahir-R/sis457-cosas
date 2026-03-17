// Copyright Epic Games, Inc. All Rights Reserved.

#include "DanmakuGameMode.h"
#include "DanmakuPawn.h"
#include "MyActor.h"
#include "Engine/World.h"
#include "MyActorFlying.h"
#include "MyActorAquatic.h"
#include "MyActorGround.h"

ADanmakuGameMode::ADanmakuGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ADanmakuPawn::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
}

void ADanmakuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();

	if (World)
	{
		for (int32 i = 0; i < ActorCount; i++) 
		{
			TSubclassOf<AMyActor> ClassToSpawn;
			float RandX = FMath::RandRange(MinLocation, MaxLocation);
			float RandY = FMath::RandRange(MinLocation, MaxLocation);
			FVector RandLoc(RandX, RandY, 150.0f);

			int32 RandType = FMath::RandRange(0, 2);
			if (RandType == 0) ClassToSpawn = AMyActorFlying::StaticClass();
			else if (RandType == 1) ClassToSpawn = AMyActorAquatic::StaticClass();
			else ClassToSpawn = AMyActorGround::StaticClass();
				AMyActor* NewActor = World->SpawnActor<AMyActor>(ClassToSpawn, RandLoc, Rotation);
		
			if (NewActor) SpawnedActors.Add(NewActor);
		}
	}
}

void ADanmakuGameMode::NotifyImpact(AActor* HitActor)
{
	if (!HitActor) return;

	TargetToSurround = HitActor;
	bWaitingLand = true;

	LandingDelay = 0.5f;
}

void ADanmakuGameMode::ExecuteSurround()
{
	if (!TargetToSurround) return;
	
	FVector Center = TargetToSurround->GetActorLocation();
	float Radius = 1000.0f;
	int32 TotalActors = SpawnedActors.Num();
	

	for (int32 i = 0; i < TotalActors; i++)
	{
		float Angle = i * (2.0f * PI / TotalActors);
		FVector Offset(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.0f);
		FVector TargetPos = Center + Offset;
		TargetPos.Z = SpawnedActors[i]->GetActorLocation().Z;
		SpawnedActors[i]->MoveToCircle(TargetPos);
	}
}

void ADanmakuGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHasLinedUp)
	{
		FormationTimer -= DeltaTime;
		if (FormationTimer <= 0.0f)
		{
			ExecuteLineUp();
			bHasLinedUp = true;
			bIsWaitingInLine = true;
			FormationTimer = 5.0f;
		}
	} 
	else if (bIsWaitingInLine)
	{
		FormationTimer -= DeltaTime;
		if (FormationTimer <= 0.0f)
		{
			ReleaseFormation();
			bIsWaitingInLine = false;
		}
	}

	if (bWaitingLand && TargetToSurround)
	{
		if (LandingDelay > 0.0f)
		{
			LandingDelay -= DeltaTime;
			return;
		}

		if (TargetToSurround->GetVelocity().Size() < 1.0f)
		{
			ExecuteSurround();
			bWaitingLand = false;
			TargetToSurround = nullptr;
		}
	}
}

void ADanmakuGameMode::ExecuteLineUp()
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn || SpawnedActors.Num() == 0) return;

	FVector PlayerLoc = PlayerPawn->GetActorLocation();
	FVector Forward = PlayerPawn->GetActorForwardVector();
	FVector Right = PlayerPawn->GetActorRightVector();

	float Spacing = 200.0f;
	float StartDistance = 300.0f;
	int32 Rows = 4, Cols = 5;

	for (int32 i = 0; i < SpawnedActors.Num(); i++)
	{
		int32 Row = i / Cols;
		int32 Col = i % Cols;

		FVector TargetPos = PlayerLoc + (Forward * StartDistance) + (Forward * Row * Spacing) + (Right * (Col - 2) * Spacing);
		SpawnedActors[i]->MoveToFormation(TargetPos, true);
	}
}

void ADanmakuGameMode::ReleaseFormation()
{
	for (AMyActor* Actor : SpawnedActors)
		if (Actor) Actor->ResumeAutoMovement();
}