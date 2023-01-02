// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "TpsPlayer.h"
#include "Enemy.h"

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
	//시간을 흐르게 한다.
	currTime += GetWorld()->GetDeltaSeconds();
	//2초가 지나면
	if (currTime > idleDelayTime)
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

}

void UEnemyFSM::UpdateDamaged()
{

}

void UEnemyFSM::UpdateDie()
{

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

}
