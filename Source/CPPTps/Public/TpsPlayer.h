// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TpsPlayer.generated.h"


UCLASS()
class CPPTPS_API ATpsPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATpsPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* compArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* compCam;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* compRifle;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compSniper;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> cameraShake;

	UPROPERTY(EditAnywhere)
	class UPlayerMove* playerMove;
	UPROPERTY(EditAnywhere)
	class UPlayerFire* playerFire;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainUI> mainUIFactory;
	UPROPERTY(EditAnywhere)
	class UMainUI* mainUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMiniMap> miniMapFactory;
	
	

	//총을 쐈는지?
	bool bFire = false;
	//카메라 흔들리고 있는 시간
	float currCamShakeTime = 0;
	//카메라 흔들려야한는 시간
	float camShakeTime = 0.5f;

	float currHP = 0;
	float maxHP = 50;

public:
	void OnHit(float damage);
	
};
