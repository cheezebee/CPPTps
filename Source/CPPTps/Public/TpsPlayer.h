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
	TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> sniperUIFactory;

	UPROPERTY()
	class UUserWidget* sniperUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> commUIFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> cameraShake;


	UPROPERTY()
	class UUserWidget* commonUI;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* exploEffect;

	//W, A, S, D 키에 따른 값 셋팅
	float h = 0;
	float v = 0;

	//마우스 움직임에 따른 값 셋팅
	float mx = 0;
	float my = 0;

	//걷기 속력
	float walkSpeed = 350;
	//뛰기 속력
	float runSpeed = 700;

	//총을 쐈는지?
	bool bFire = false;
	//카메라 흔들리고 있는 시간
	float currCamShakeTime = 0;
	//카메라 흔들려야한는 시간
	float camShakeTime = 0.5f;

public:

	void MoveAction(float deltaTime);
	void RotateAction();

	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputLookUp(float value);
	void InputTurn(float value);
	void InputJump();
	void InputFire();
	void InputRifle();
	void InputSniper();
	void InputRun();
	void ChangeWeapon(bool useSniper);

	void InputZoomIn();
	void InputZoomOut();
};
