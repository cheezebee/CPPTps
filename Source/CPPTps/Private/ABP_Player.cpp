// Fill out your copyright notice in the Description page of Project Settings.


#include "ABP_Player.h"
#include "TpsPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

UABP_Player::UABP_Player()
{
	//attack montage 가져오자
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/Blueprints/AMT_Attack.AMT_Attack'"));
	if (tempMontage.Succeeded())
	{
		attackAnimMontage = tempMontage.Object;
	}
}

void UABP_Player::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	//ATpsPlayer 저장해 놓자
	//1. 소유 폰 가져오기
	APawn* pawn = TryGetPawnOwner();

	//2. ATpsPlayer 로 캐스팅
	me = Cast<ATpsPlayer>(pawn);
}

void UABP_Player::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);		

	//만약에 me 가 nullptr 아니면
	if (me != nullptr)
	{    
		//3. 속도 , 앞방향, 오른쪽 방향 가져오기
		FVector velocity = me->GetVelocity();
		FVector forward = me->GetActorForwardVector();
		FVector right = me->GetActorRightVector();

		//4. 두 벡터를 내적한 값 dirV 에 셋팅 (앞으로 가고 있는지? 뒤로 가고 있는지?)
		dirV = FVector::DotProduct(velocity, forward);		

		//5. 오른쪽으로 가고 있는지? 왼쪽으로 가고 있는지?
		dirH = FVector::DotProduct(velocity, right);

		//공중에 있는지 여부 셋팅
		bAir = me->GetCharacterMovement()->IsFalling();
	}
}

void UABP_Player::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}