// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

public:
	//걷기 속력
	float walkSpeed = 350;
	//뛰기 속력
	float runSpeed = 700;

	//W, A, S, D 키에 따른 값 셋팅
	float h = 0;
	float v = 0;

	//마우스 움직임에 따른 값 셋팅
	float mx = 0;
	float my = 0;

public:
	void MoveAction(float deltaTime);
	void RotateAction();

	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputLookUp(float value);
	void InputTurn(float value);
	void InputJump();
	void InputRun();
};
