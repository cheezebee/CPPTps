// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/Image.h>

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	//text hp (수치) 가져오자
	currHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("_currHP")));

	//progress bar hp 가져오자
	currHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("_currHPBar")));

	//rifle 이미지와 sniper 이미지 가져오자
	rifle = Cast<UImage>(GetWidgetFromName(TEXT("_rifle")));
	sniper = Cast<UImage>(GetWidgetFromName(TEXT("_sniper")));
}

void UMainUI::UpdateCurrHP(float curr, float max)
{
	//hp 를 0 ~ 1 사이로 표현
	float hp = curr / max;

	//수치 UI 갱신
	FString strHP = FString::Printf(TEXT("%d%%"), (int32)(hp * 100));
	currHP->SetText(FText::FromString(strHP));
	
	//Progress bar UI 갱신
	currHPBar->SetPercent(hp);
}

void UMainUI::UpdateWeapon(bool useSniper)
{
	SetZOrderWeapon(useSniper, sniper);
	SetZOrderWeapon(!useSniper, rifle);
}

void UMainUI::SetZOrderWeapon(bool bFoucs, class UImage* img)
{
	//CanvasPanelSlot 을 가져오자
	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(img->Slot);
	

	//만약에 bFoucs 가 true
	if (bFoucs == true)
	{
		//zorder 를 2로
		slot->SetZOrder(2);	
	}
	//그렇지 않으면
	else
	{
		//zorder 를 0로
		slot->SetZOrder(0);
	}
}