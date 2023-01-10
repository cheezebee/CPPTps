// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"


void UEnemyAnim::AnimNotify_DieEnd()
{
	//GetOwningActor(); <<-------- Actor 를 가져온다.
	/*AActor* actor = GetOwningActor();
	APawn* p =Cast<APawn>(actor);*/

	//1. 내가 붙어 있는 Pawn 을 가져온다.
	APawn* pawn = TryGetPawnOwner();

	//2. AEnemy 로 형변환한다.
	AEnemy* enemy = Cast<AEnemy>(pawn);

	//3. enemy 가 가지고 있는 fsm 을 이용해서 bDieMove 를 true
	enemy->fsm->bDieMove = true;

	//Cast<AEnemy>(TryGetPawnOwner())->fsm->bDieMove = true;
}

void UEnemyAnim::AnimNotify_EnemyAttack()
{
	//1. 공격!!! 출력하자
	UE_LOG(LogTemp, Error, TEXT("Attack!!!"));
}