// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyActor.h"
#include "DanmakuGameMode.generated.h"

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
	AActor* TargetToSurround;
	bool bWaitingLand;
	float LandingDelay = 0.0f;

	int32 ActorCount = 20;
	float MinLocation = -1000.0f;
	float MaxLocation= 1000.0f;
	FRotator Rotation = FRotator::ZeroRotator;

	void ExecuteSurround();
	
	float FormationTimer = 5.0f;
	bool bHasLinedUp = false;
	bool bIsWaitingInLine = false;

	void ExecuteLineUp();
	void ReleaseFormation();

};