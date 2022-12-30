// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"

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

	// ...
	
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

	//���� ���� ��� ����غ���

	UE_LOG(LogTemp, Warning, TEXT("%d"), currState);

	/*UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *enumPtr->GetNameStringByIndex((int32)currState));
	}*/
}

void UEnemyFSM::UpdateIdle()
{
	//�ð��� �帣�� �Ѵ�.
	currTime += GetWorld()->GetDeltaSeconds();
	//2�ʰ� ������
	if (currTime > idleDelayTime)
	{
		//������¸� Move�� �Ѵ�.
		currState = EEnemyState::Move;
	}
}

void UEnemyFSM::UpdateMove()
{

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
