// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UMainUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	UPROPERTY()
	class UImage* hit;

	bool bHit = false;
	float currTime = 0;

	UPROPERTY()
	class UTextBlock* currHP;

	UPROPERTY()
	class UProgressBar* currProgress;

	UPROPERTY()
	class UImage* rifle;
	UPROPERTY()
	class UImage* sniper;

public:
	void UpdateCurrHP(float curr, float max);
	void UpdateWeapon(bool useSniper);
	void ShowWeapon(bool visible, class UImage* img);
};
