// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorFlying.h"

AMyActorFlying::AMyActorFlying()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus'"));
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
	}

	MovementSpeed = 1000.0f;
}
	
void AMyActorFlying::BeginPlay()

{
	FVector Loc = GetActorLocation();
	SetActorLocation(FVector(Loc.X, Loc.Y, 150.0f));
	Super::BeginPlay();
};

