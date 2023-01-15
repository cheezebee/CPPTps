// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include <Components/Image.h>
#include <Components/CanvasPanelSlot.h>

bool UMainUI::Initialize()
{
	bool returnValue = Super::Initialize();

	hit = Cast<UImage>(GetWidgetFromName(TEXT("_hit")));

	currHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("currHP")));

	currProgress = Cast<UProgressBar>(GetWidgetFromName(TEXT("_currProgress")));

	rifle = Cast<UImage>(GetWidgetFromName(TEXT("_Rifle")));
	sniper = Cast<UImage>(GetWidgetFromName(TEXT("_Sniper")));

	return returnValue;
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bHit)
	{
		currTime += InDeltaTime;
		if (currTime > 0.3f)
		{
			bHit = false;
			hit->SetRenderOpacity(0);
		}		
	}	
}

void UMainUI::UpdateCurrHP(float curr, float max)
{
	float per = curr / max;

	FString strHP = FString::Printf(TEXT("%d%%"), (int32)(per*100));
	currHP->SetText(FText::FromString(strHP));

	currProgress->SetPercent(per);

	bHit = true;
	hit->SetRenderOpacity(1.0f);
	currTime = 0;
}

void UMainUI::UpdateWeapon(bool useSniper)
{
	ShowWeapon(useSniper, sniper);
	ShowWeapon(!useSniper, rifle);
	
}

void UMainUI::ShowWeapon(bool visible, class UImage* img)
{
	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(img->Slot);
	if (visible)
	{
		slot->SetZOrder(2);
	}
	else
	{
		slot->SetZOrder(0);
	}
}
