// Fill out your copyright notice in the Description page of Project Settings.


#include "ABP_TspPlayer.h"
#include "TpsPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

UABP_TspPlayer::UABP_TspPlayer()
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/Blueprints/AM_Attack.AM_Attack'"));
	if (tempMontage.Succeeded())
	{
		attackAnimMontage = tempMontage.Object;
	}
}

void UABP_TspPlayer::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	me = Cast<ATpsPlayer>(TryGetPawnOwner());
}

void UABP_TspPlayer::NativeUpdateAnimation(float DeltaSeconds)
{
	if (me != nullptr)
	{
		//speed = me->GetVelocity().Length();
		speed = FVector::DotProduct(me->GetActorForwardVector(), me->GetVelocity()); 
		dirH = FVector::DotProduct(me->GetActorRightVector(), me->GetVelocity());
		//UE_LOG(LogTemp, Warning, TEXT("%f"), speed);

		bAir = me->GetCharacterMovement()->IsFalling();
	}		
}

void UABP_TspPlayer::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}