// Fill out your copyright notice in the Description page of Project Settings.


#include "Minimap.h"
#include <Components/SceneCaptureComponent2D.h>
#include <Engine/TextureRenderTarget2D.h>
#include <Kismet/GameplayStatics.h>
#include "TpsPlayer.h"

AMinimap::AMinimap()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> tempTex(TEXT("TextureRenderTarget2D'/Game/Images/RT_Minimap.RT_Minimap'"));
	if (tempTex.Succeeded())
	{
		GetCaptureComponent2D()->TextureTarget = tempTex.Object;
	}
}

void AMinimap::BeginPlay()
{
	Super::BeginPlay();

	//플레이어 찾자
	target = UGameplayStatics::GetActorOfClass(GetWorld(), ATpsPlayer::StaticClass());
}

void AMinimap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//target 위치를 pos 지역변수에 가져오자
	FVector pos = target->GetActorLocation();
	//pos 의 Z 값을 540 으로 변경
	pos.Z = 540;
	//나의 위치를 pos 값으로 설정한다.
	SetActorLocation(pos);
}