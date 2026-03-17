// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorGround.h"

AMyActorGround::AMyActorGround()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
	}
	MovementSpeed = 500.0f;
}