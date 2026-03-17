// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorAquatic.h"

AMyActorAquatic::AMyActorAquatic()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'"));
	if (MeshAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshAsset.Object);
	}
	MovementSpeed = 350.0f;
}