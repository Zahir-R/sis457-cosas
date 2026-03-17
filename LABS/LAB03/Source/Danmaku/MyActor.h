// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class DANMAKU_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UStaticMeshComponent* MeshComponent;
	float MovementSpeed = 100.0f;

	FVector WorldLimitsMin = FVector(-1000, -1000, 0);
	FVector WorldLimitsMax = FVector(1000, 1000, 0);

	void MoveToCircle(FVector TargetLocation);
	void MoveToFormation(FVector TargetLocation, bool bManualResume = false);
	void ResumeAutoMovement();


protected:
	TArray<FVector> WayPoints;
	int32 IndexWayPoint;
	FVector InitialPosition;
	float Tolerance = 50.0f;

	bool bInCircleMode = false;
	float WaitTimer = 0.0f;
	FVector CircleLocation;

	float OriginalMovementSpeed;
	bool bWaitingForManualResume = false;

	bool bIsCurving = false;
	FVector CurveStartLocation;
	FVector CurveControlPoint;
	float CurveAlpha = 0.0f;
	float CurveSpeed = 1.0f;
};
