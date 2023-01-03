// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "TpsPlayer.h"
#include "Enemy.h"
#include <Components/CapsuleComponent.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//타겟 찾자
	target = Cast<ATpsPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATpsPlayer::StaticClass()));
	//나를 찾자
	me = Cast<AEnemy>(GetOwner());	

	//나의 초기 체력을 셋팅하자
	currHP = maxHP;
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (currState)
	{
	case EEnemyState::Idle:
		UpdateIdle();
		break;
	case EEnemyState::Move:
		UpdateMove();
		break;
	case EEnemyState::Attack:
		UpdateAttack();
		break;
	case EEnemyState::Damaged:
		UpdateDamaged();
		break;
	case EEnemyState::Die:
		UpdateDie();
		break;
	}
}

void UEnemyFSM::UpdateIdle()
{
	//idleDelayTime 이 지나면	
	if (IsWaitComplete(idleDelayTime))
	{
		//현재상태를 Move 로 한다.
		ChangeState(EEnemyState::Move);
	}
}

void UEnemyFSM::UpdateMove()
{
	//1. 타겟을 향하는 방향을 구하고(target - me)
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	
	//만약에 target - me 거리가 공격범위보다 작으면
	if (dir.Length() < attackRange)
	{
		//상태를 Attack 으로 변경
		ChangeState(EEnemyState::Attack);
	}
	//그렇지 않으면
	else
	{
		//2. 그 방향으로 이동하고 싶다.
		me->AddMovementInput(dir.GetSafeNormal());
	}
}

void UEnemyFSM::UpdateAttack()
{	
	//2. 만약에 현재시간이 attackDelayTime 보다 커지면
	if (IsWaitComplete(attackDelayTime))
	{
		//3. target 과 me 거리를 구하자.
		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();
		//4. 만약에 그거리가 attackRange보다 작으면
		if (dist < attackRange)
		{
			//5. 공격!!! 출력하자
			UE_LOG(LogTemp, Error, TEXT("Attack!!!"));
		}
		else
		{
			//6. 그렇지 않으면 Idle 상태로 가자
			ChangeState(EEnemyState::Idle);
		}
	}
}

void UEnemyFSM::UpdateDamaged()
{
	//damageDelayTime 이 지나면
	if (IsWaitComplete(damageDelayTime))
	{
		//Move 상태
		ChangeState(EEnemyState::Move);
	}
}

void UEnemyFSM::UpdateDie()
{
	//P = P0 + vt
	//1. 아래로 내려가는 위치를 구한다.
	FVector p0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;

	//2. 만약에 p.Z 가 -200 보다 작으면 파괴한다
	if (p.Z < -200)
	{
		me->Destroy();
	}
	//3. 그렇지 않으면 해당 위치로 셋팅한다
	else
	{
		me->SetActorLocation(p);
	}

}

void UEnemyFSM::ChangeState(EEnemyState state)
{
	//상태 변경 로그를 출력하자
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)state));
	}

	//현재 상태를 갱신
	currState = state;

	//상태에 따른 초기설정
	switch (currState)
	{
	case EEnemyState::Attack:
		currTime = attackDelayTime;
		break;
	case EEnemyState::Die:		
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void UEnemyFSM::ReceiveDamage()
{
	//피를 줄이자
	currHP--;
	//hp 가 0보다 크면 Damage 상태로 전환
	if (currHP > 0)
	{
		ChangeState(EEnemyState::Damaged);
	}
	//그렇지 않으면 Die 상태로 전환
	else
	{

		ChangeState(EEnemyState::Die);
	}
}

bool UEnemyFSM::IsWaitComplete(float delayTime)
{
	//1. 시간을 흐르게 한다.
	currTime += GetWorld()->DeltaTimeSeconds;
		
	//2. 만약에 현재시간이 delayTime보다 커지면 
	if (currTime > delayTime)
	{
		//3. 현재시간을 0으로 초기화
		currTime = 0;
		//4. true 반환
		return true;
	}

	//5. 그렇지 않으면 false 를 반환
	return false;
}
