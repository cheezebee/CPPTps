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

	//W, A, S, D Ű�� ���� �� ����
	float h = 0;
	float v = 0;

	//���콺 �����ӿ� ���� �� ����
	float mx = 0;
	float my = 0;

	float walkSpeed = 600;

public:

	void MoveAction(float deltaTime);
	void RotateAction();

	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputLookUp(float value);
	void InputTurn(float value);
	void InputJump();
};