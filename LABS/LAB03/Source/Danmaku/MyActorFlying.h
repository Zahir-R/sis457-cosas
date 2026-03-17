// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyActor.h"
#include "MyActorFlying.generated.h"

/**
 * 
 */
UCLASS()
class DANMAKU_API AMyActorFlying : public AMyActor
{
	GENERATED_BODY()
	
public:
	AMyActorFlying();

protected:
	virtual void BeginPlay() override;

};
