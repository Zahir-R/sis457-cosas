// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemigo.generated.h"

class UStaticMeshComponent;

UCLASS()
class NAVESUSFX_012026_API AEnemigo : public AActor
{
	GENERATED_BODY()

public:
	AEnemigo();

	virtual void Tick(float DeltaTime) override;
	UStaticMeshComponent* Mesh;
	
	float MovementSpeed = 100.0f;

	FVector WorldLimitsMin = FVector(-1000.0f, -1000.0f, 0.0f);
	FVector WorldLimitsMax = FVector(1000.0f, 1000.0f, 0.0f);

	void Move();

protected:
	virtual void BeginPlay() override;

	TArray<FVector> WayPoints;
	int32 IndexWayPoint;
	FVector InitialPosition;
	float Tolerance = 50.0f;
};