// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	UPlayerFire();
	
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

public:
	

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> sniperUIFactory;

	UPROPERTY()
		class UUserWidget* sniperUI;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> commUIFactory;

	UPROPERTY()
		class UUserWidget* commonUI;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* exploEffect;

	//총을 쐈는지?
	bool bFire = false;
	//카메라 흔들리고 있는 시간
	float currCamShakeTime = 0;
	//카메라 흔들려야한는 시간
	float camShakeTime = 0.5f;

public:
	void InputFire();
	void InputRifle();
	void InputSniper();

	void ChangeWeapon(bool useSniper);

	void InputZoomIn();
	void InputZoomOut();
};
