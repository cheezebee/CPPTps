// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "MiniMap.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API AMiniMap : public ASceneCapture2D
{
	GENERATED_BODY()
	
public:
	AMiniMap();

public:
	void Init(AActor* target);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	AActor* followTarget;
};
