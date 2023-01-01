// Fill out your copyright notice in the Description page of Project Settings.


#include "ABP_TspPlayer.h"
#include "TpsPlayer.h"

void UABP_TspPlayer::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	me = Cast<ATpsPlayer>(TryGetPawnOwner());
}

void UABP_TspPlayer::NativeUpdateAnimation(float DeltaSeconds)
{
	if (me != nullptr)
	{
		speed = me->GetVelocity().Length();
	}
}