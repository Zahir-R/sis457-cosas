// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LAB00GameMode.generated.h"

UCLASS(MinimalAPI)
class ALAB00GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALAB00GameMode();

protected:
	virtual void BeginPlay() override;
};




