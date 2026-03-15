// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LAB00Projectile.h"
#include "Enemy.generated.h"

class UStaticMeshComponent;

UCLASS()
class LAB00_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();
	UStaticMeshComponent* EnemyMesh;

	float MovementSpeed = 100.0f;
	FVector MaxWorldLimits = FVector(1000, 1000, 0);
	FVector MinWorldLimits = FVector(-1000, -1000, 0);
		
	float FireRate = 2.0f;
	TSubclassOf<class ALAB00Projectile> ProjectileClass;

private:
	TArray<FVector> Waypoints;
	int32 WaypointIndex = 0;

	FVector InitialPosition;
	float Tolerance = 50.0f;
	FTimerHandle TimerHandleShoot;
	void ShootAtPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
