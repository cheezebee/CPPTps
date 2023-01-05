// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABP_Player.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UABP_Player : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:

	//Player Actor
	UPROPERTY()
	class ATpsPlayer* me;

	// 앞, 뒤를 판별하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float dirV = 0;
	
	//공중에 있는지 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAir = false;
};
