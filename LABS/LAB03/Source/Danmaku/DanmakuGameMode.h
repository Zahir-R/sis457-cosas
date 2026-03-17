// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyActor.h"
#include "DanmakuGameMode.generated.h"

enum class ESwarmState : uint8
{
	InitialLineUp,
	WaitingInLine,
	Roaming,
	Surrounding,
	Researching
};

UCLASS(MinimalAPI)
class ADanmakuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADanmakuGameMode();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void NotifyImpact(AActor* HitActor);
	
private:
	TArray<AMyActor*> SpawnedActors;
	int32 ActorCount = 20;

	float MinLocation = -1000.0f;
	float MaxLocation= 1000.0f;

	AActor* TargetToSurround;
	float LandingDelay = 0.0f;
	void ExecuteSurround();

	FRotator Rotation = FRotator::ZeroRotator;
	
	float FormationTimer = 5.0f;
	
	ESwarmState CurrentState = ESwarmState::InitialLineUp;

	void ExecuteLineUp();
	void ReleaseFormation();
};