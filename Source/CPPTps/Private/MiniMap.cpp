// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMap.h"
#include <Components/SceneCaptureComponent2D.h>
#include <Engine/TextureRenderTarget2D.h>

AMiniMap::AMiniMap()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> tempTex(TEXT("TextureRenderTarget2D'/Game/RT_Minimap.RT_Minimap'"));
	if (tempTex.Succeeded())
	{
		GetCaptureComponent2D()->TextureTarget = tempTex.Object;
	}
}

void AMiniMap::Init(AActor* target)
{
	followTarget = target;

	SetActorRotation(FRotator(-90, 0, 0));
}

void AMiniMap::Tick(float DeltaTime)
{
	FVector pos = followTarget->GetActorLocation();
	pos.Z = 480;
	SetActorLocation(pos);
}