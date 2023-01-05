// Fill out your copyright notice in the Description page of Project Settings.


#include "ABP_Player.h"
#include "TpsPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UABP_Player::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	//ATpsPlayer ������ ����
	//1. ���� �� ��������
	APawn* pawn = TryGetPawnOwner();

	//2. ATpsPlayer �� ĳ����
	me = Cast<ATpsPlayer>(pawn);
}

void UABP_Player::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);		

	//���࿡ me �� nullptr �ƴϸ�
	if (me != nullptr)
	{    
		//3. �ӵ� , �չ��� ��������
		FVector velocity = me->GetVelocity();
		FVector forward = me->GetActorForwardVector();

		//4. �� ���͸� ������ �� dirV �� ����
		dirV = FVector::DotProduct(velocity, forward);		

		//���߿� �ִ��� ���� ����
		bAir = me->GetCharacterMovement()->IsFalling();
	}
}