// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABP_TspPlayer.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UABP_TspPlayer : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed = 0;

	UPROPERTY()
	class ATpsPlayer* me;
};
