#include "Enemigo.h"
#include "Components/StaticMeshComponent.h"

AEnemigo::AEnemigo()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));
	if (MeshAsset.Succeeded()) Mesh->SetStaticMesh(MeshAsset.Object);
}

void AEnemigo::BeginPlay()
{
	Super::BeginPlay();
	InitialPosition = GetActorLocation();
	WayPoints.Empty();
	WayPoints.Add(InitialPosition);

	for (int32 i = 0; i < 10; i++)
	{
		float X = FMath::RandRange(WorldLimitsMin.X, WorldLimitsMax.X);
		float Y = FMath::RandRange(WorldLimitsMin.Y, WorldLimitsMax.Y);
	
		FVector NewWayPoint(X, Y, InitialPosition.Z);
		WayPoints.Add(NewWayPoint);
	}
	IndexWayPoint = 1;
}

void AEnemigo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
}

void AEnemigo::Move()
{
	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = WayPoints[IndexWayPoint];
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	FVector NewLocation = CurrentLocation + Direction * MovementSpeed * GetWorld()->DeltaTimeSeconds;
	SetActorLocation(NewLocation);

	float DistanceToTarget = FVector::Dist(CurrentLocation, TargetLocation);

	if (DistanceToTarget <= Tolerance)
	{
		IndexWayPoint++;
		if (IndexWayPoint >= WayPoints.Num())
		{
			IndexWayPoint = 0;
		}
	}
}