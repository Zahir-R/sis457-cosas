// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMMesh"));
	RootComponent = EnemyMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));

	if (MeshAsset.Succeeded()) EnemyMesh->SetStaticMesh(MeshAsset.Object);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	InitialPosition = GetActorLocation();
	Waypoints.Empty();

	Waypoints.Add(InitialPosition);

	for (int32 i = 0; i < 10; i++)
	{
		float X = FMath::RandRange(MinWorldLimits.X, MaxWorldLimits.X);
		float Y = FMath::RandRange(MinWorldLimits.Y, MaxWorldLimits.Y);
		FVector NewWaypoint(X, Y, InitialPosition.Z);
		Waypoints.Add(NewWaypoint);
	}

	WaypointIndex = 1;

	GetWorldTimerManager().SetTimer(TimerHandleShoot, this, &AEnemy::ShootAtPlayer, FireRate, true);
}

void AEnemy::ShootAtPlayer()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		FVector ShootingDirection = PlayerPawn->GetActorLocation() - GetActorLocation();
		FRotator FireRotation = ShootingDirection.Rotation();
		FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 30.0f) + (FireRotation.Vector() * 200.0f);

		UWorld* const World = GetWorld();
		if (World)
		{
			World->SpawnActor<ALAB00Projectile>(ALAB00Projectile::StaticClass(), SpawnLocation, FireRotation);
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentTarget = Waypoints[WaypointIndex];
	FVector Direction = CurrentTarget - GetActorLocation();
	float DistanteToTarget = Direction.Size();

	if (DistanteToTarget > Tolerance)
	{
		Direction.Normalize();
		AddActorWorldOffset(Direction * MovementSpeed * DeltaTime);
	}
	else
	{
		WaypointIndex = (WaypointIndex + 1) % Waypoints.Num();
	}
}

