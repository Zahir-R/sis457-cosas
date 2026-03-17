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

	OriginalMovementSpeed = MovementSpeed;
	MovementSpeed = 1500;

	bInCircleMode = true;
	bIsCurving = true;
	CurveAlpha = 0.0f;
	WaitTimer = 0.0f;
}

void AMyActor::MoveToFormation(FVector TargetLocation, bool bManualResume)
{
	MoveToCircle(TargetLocation);
	bWaitingForManualResume = bManualResume;
}

void AMyActor::ResumeAutoMovement()
{
	MovementSpeed = OriginalMovementSpeed;
	bInCircleMode = false;
	bWaitingForManualResume = false;
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bInCircleMode)
	{
		if (bIsCurving)
		{
			CurveAlpha += (MovementSpeed / FVector::Dist(CurveStartLocation, CircleLocation)) * DeltaTime;
		
			if (CurveAlpha >= 1.0f)
			{
				SetActorLocation(CircleLocation);
				bIsCurving = false;
			}
			else
			{
				float T = CurveAlpha;
				FVector NewLoc = FMath::Square(1.0f - T) * CurveStartLocation + 2.0f * (1.0f - T) * T * CurveControlPoint + FMath::Square(T) * CircleLocation;

				SetActorLocation(NewLoc);
			}
		}
		else {
			WaitTimer += DeltaTime;
			if (WaitTimer >= 2.0f) {
				MovementSpeed = OriginalMovementSpeed;
				bInCircleMode = false;
			}
		}
	}

	else if (WayPoints.Num() > 1)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector FinalLocation = WayPoints[IndexWayPoint];

		float Distance = FVector::Dist(CurrentLocation, FinalLocation);

		if (Distance <= Tolerance)
		{
			IndexWayPoint++;
			if (IndexWayPoint >= WayPoints.Num())
			{
				IndexWayPoint = 0;
			}
		}
		else
		{
			FVector Direction = (FinalLocation - CurrentLocation).GetSafeNormal();
			FVector NewLocation = CurrentLocation + (Direction * MovementSpeed * DeltaTime);
			SetActorLocation(NewLocation);
		}
	}
}

