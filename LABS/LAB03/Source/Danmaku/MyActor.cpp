// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
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

void AMyActor::MoveToCircle(FVector TargetLocation, float ArcMultiplier)
{
	CircleLocation = TargetLocation;
	CurveStartLocation = GetActorLocation();

	float DistanceToTarget = FVector::Dist(CurveStartLocation, CircleLocation);
	FVector MidPoint = (CurveStartLocation + CircleLocation) * 0.5f;
	FVector Direction = (CircleLocation - CurveStartLocation).GetSafeNormal();

	FVector SideOffset = FVector::CrossProduct(FVector::UpVector, Direction) * (DistanceToTarget * 0.5f * ArcMultiplier);

	if (SideOffset.SizeSquared() > 0.1f)
	{
		FVector TowardCenter = -MidPoint;
		TowardCenter.Z = 0;

		if (FVector::DotProduct(SideOffset, TowardCenter) > 0.0f) SideOffset *= -1.0f;
	}

	CurveControlPoint = MidPoint + SideOffset;

	if (CurrentState != EMovementState::MovingTo) OriginalMovementSpeed = MovementSpeed;

	MovementSpeed = 1500;
	CurveAlpha = 0.0f;

	WaitTimer = 0.0f;

	CurrentState = EMovementState::MovingTo;
}

void AMyActor::MoveToArc(FVector Center, FVector Target, float Radius, float TotalAngleDegree, int32 ActorIndex, int32 TotalActors)
{
	FVector DirToTarget = (Target - Center).GetSafeNormal();

	FVector SideDir = FVector::CrossProduct(FVector::UpVector, DirToTarget);

	float SpreadFraction = (TotalActors > 1) ? ((float)ActorIndex / (TotalActors - 1)) : 0.5f;
	float VisualOffset = (SpreadFraction * 2.0f - 1.0f);
	float HorizontalSpacing = 500.0f;
	float ForwardAmount = FMath::Abs(VisualOffset) * 300.0f;

	FVector Offset = (SideDir * (VisualOffset * HorizontalSpacing)) + (DirToTarget * ForwardAmount);

	FVector ArcDestination = Center + Offset;

	MoveToCircle(ArcDestination, 0.4f);
}

void AMyActor::MoveToFormation(FVector TargetLocation, bool bManualResume)
{
	MoveToCircle(TargetLocation);
	if (bManualResume) CurrentState = EMovementState::MovingTo;
}

void AMyActor::ResumeAutoMovement()
{
	MovementSpeed = OriginalMovementSpeed;
	CurrentState = EMovementState::Roaming;
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	switch (CurrentState)
	{
	case EMovementState::Roaming:
	{
		if (WayPoints.Num() <= 1) break;

		FVector CurrentLocation = GetActorLocation();
		FVector FinalLocation = WayPoints[IndexWayPoint];

		if (FVector::Dist(CurrentLocation, FinalLocation) <= Tolerance)
		{
			IndexWayPoint = (IndexWayPoint + 1) % WayPoints.Num();
		}
		else
		{
			FVector Direction = (FinalLocation - CurrentLocation).GetSafeNormal();
			SetActorLocation(CurrentLocation + (Direction * MovementSpeed * DeltaTime));
		}
		break;
	}
	case EMovementState::MovingTo:
	{
		CurveAlpha += (MovementSpeed / FVector::Dist(CurveStartLocation, CircleLocation)) * DeltaTime;

		if (CurveAlpha >= 1.0f)
		{
			SetActorLocation(CircleLocation);
			MovementSpeed = OriginalMovementSpeed;
		}
		else
		{
			float T = CurveAlpha;
			FVector NewLoc = FMath::Square(1.0f - T) * CurveStartLocation + 2.0f * (1.0f - T) * T * CurveControlPoint + FMath::Square(T) * CircleLocation;
			SetActorLocation(NewLoc);
		}
		break;
	}
	case EMovementState::Stationary:
	case EMovementState::ManualWait:
		break;
	}
}

