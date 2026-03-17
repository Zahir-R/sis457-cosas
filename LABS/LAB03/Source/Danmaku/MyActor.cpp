// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Components/StaticMeshComponent.h"
//#include "UObject/ConstructorHelpers.h"

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
	/*
	MeshComponent->SetVisibility(true);

	MeshComponent->SetHiddenInGame(false);

	NumDirections = 20;
	AngleStep = 2 * PI / NumDirections;
	CurrentAngle = 0.f;

	MoveSpeed = 200.f;
	MaxDistance = 300.f;
	DistanceTraveled = 0.f;
	DirectionIndex = 0;

	CurrentDirection = FVector(0.f, -1.f, 0.f);
*/}

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

void AMyActor::MoveToCircle(FVector TargetLocation)
{
	CircleLocation = TargetLocation;
	CurveStartLocation = GetActorLocation();

	FVector MidPoint = (CurveStartLocation + CircleLocation) * 0.5f;
	FVector Direction = (CircleLocation - CurveStartLocation).GetSafeNormal();

	FVector SideOffset = FVector::CrossProduct(FVector::UpVector, Direction) * 500.0f;

	if (FMath::RandBool()) SideOffset *= -1.0f;

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
		/* }

		FVector CurrentLocation = GetActorLocation();
		CircleLocation.Z = CurrentLocation.Z;
		float Distance = FVector::Dist(CurrentLocation, CircleLocation);

		if (Distance > Tolerance)
		{
			FVector Dir = (CircleLocation - CurrentLocation).GetSafeNormal();
			SetActorLocation(CurrentLocation + (Dir * MovementSpeed * DeltaTime));
		}
		else if (!bWaitingForManualResume)
		{*/
			//WaitTimer += DeltaTime;
			//if (WaitTimer >= 2.0f) ResumeAutoMovement();
		//}
	//	else
		//{
			//WaitTimer += DeltaTime;
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
	
	
	/*
	CurrentAngle += DeltaTime * (MoveSpeed / MaxDistance);
	CurrentDirection = FVector(FMath::Cos(CurrentAngle), FMath::Sin(CurrentAngle), 0.f);

	FVector DeltaMove = CurrentDirection * MoveSpeed * DeltaTime;
	AddActorWorldOffset(DeltaMove);

	DistanceTraveled += DeltaMove.Size();
	*/
	/*if (DistanceTraveled >= MaxDistance)
	{
		DistanceTraveled = 0.f;
		DirectionIndex = (DirectionIndex + 1) % NumDirections;

		CurrentAngle = DirectionIndex * AngleStep;
		CurrentDirection = FVector(FMath::Cos(CurrentAngle), FMath::Sin(CurrentAngle), 0.f);
	}*/

}

