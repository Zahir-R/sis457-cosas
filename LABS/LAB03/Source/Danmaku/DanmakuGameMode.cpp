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
	LandingDelay = 0.5f;

	if (CurrentState == ESwarmState::Surrounding) CurrentState = ESwarmState::Roaming;
}

void ADanmakuGameMode::ExecuteSurround()
{
	if (!TargetToSurround) return;
	
	FVector Center = TargetToSurround->GetActorLocation();
	float Radius = 500.0f;
	int32 TotalActors = SpawnedActors.Num();
	
	SpawnedActors.Sort([Center](const AMyActor& A, const AMyActor& B)
		{
			FVector DirA = (A.GetActorLocation() - Center).GetSafeNormal();
			FVector DirB = (B.GetActorLocation() - Center).GetSafeNormal();

			float AngleA = FMath::Atan2(DirA.Y, DirA.X);
			float AngleB = FMath::Atan2(DirB.Y, DirB.X);
			return AngleA < AngleB;
		});

	FVector AverageDir = FVector::ZeroVector;
	for (AMyActor* Actor : SpawnedActors) AverageDir += (Actor->GetActorLocation() - Center).GetSafeNormal();
	
	float SwarmCenterAngle = FMath::Atan2(AverageDir.Y, AverageDir.X);

	for (int32 i = 0; i < TotalActors; i++)
	{
		float RelativeAngle = ((float)i / (float)TotalActors - 0.5f) * (2.0f * PI);
		float FinalAngle = SwarmCenterAngle + RelativeAngle;

		FVector Offset(FMath::Cos(FinalAngle) * Radius, FMath::Sin(FinalAngle) * Radius, 0.0f);
		FVector TargetPos = Center + Offset;
		TargetPos.Z = SpawnedActors[i]->GetActorLocation().Z;

		float ArcMultiplier = (FMath::Abs(RelativeAngle) > PI * 0.5f) ? 1.2f : 0.5f;
		SpawnedActors[i]->MoveToCircle(TargetPos, ArcMultiplier);
	}

	FormationTimer = 5.0f;
}

void ADanmakuGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
	case ESwarmState::InitialLineUp:
		FormationTimer -= DeltaTime;
		if (FormationTimer <= 0.0f)
		{
			ExecuteLineUp();
			CurrentState = ESwarmState::WaitingInLine;
			FormationTimer = 5.0f;
		}
		break;

	case ESwarmState::WaitingInLine:
		FormationTimer -= DeltaTime;

		if (TargetToSurround && LandingDelay <= 0.0f && TargetToSurround->GetVelocity().Size() < 1.0f)
		{
			ExecuteSurround();
			CurrentState = ESwarmState::Surrounding;
		}
		else if (FormationTimer <= 0.0f)
		{
			ReleaseFormation();
			CurrentState = ESwarmState::Roaming;
		}
		break;

	case ESwarmState::Roaming:
		if (TargetToSurround && LandingDelay <= 0.0f && TargetToSurround->GetVelocity().Size() < 1.0f)
		{
			ExecuteSurround();
			CurrentState = ESwarmState::Surrounding;
		}
		break;

	case ESwarmState::Surrounding:
		FormationTimer -= DeltaTime;
		if (FormationTimer <= 0.0f)
		{
			TargetToSurround = nullptr;
			CurrentState = ESwarmState::Roaming;
		}
		break;
	}

	if (LandingDelay > 0.0f) LandingDelay -= DeltaTime;
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