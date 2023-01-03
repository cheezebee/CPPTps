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

	//Ÿ�� ã��
	target = Cast<ATpsPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATpsPlayer::StaticClass()));
	//���� ã��
	me = Cast<AEnemy>(GetOwner());	

	//���� �ʱ� ü���� ��������
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
	//idleDelayTime �� ������	
	if (IsWaitComplete(idleDelayTime))
	{
		//������¸� Move �� �Ѵ�.
		ChangeState(EEnemyState::Move);
	}
}

void UEnemyFSM::UpdateMove()
{
	//1. Ÿ���� ���ϴ� ������ ���ϰ�(target - me)
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	
	//���࿡ target - me �Ÿ��� ���ݹ������� ������
	if (dir.Length() < attackRange)
	{
		//���¸� Attack ���� ����
		ChangeState(EEnemyState::Attack);
	}
	//�׷��� ������
	else
	{
		//2. �� �������� �̵��ϰ� �ʹ�.
		me->AddMovementInput(dir.GetSafeNormal());
	}
}

void UEnemyFSM::UpdateAttack()
{	
	//2. ���࿡ ����ð��� attackDelayTime ���� Ŀ����
	if (IsWaitComplete(attackDelayTime))
	{
		//3. target �� me �Ÿ��� ������.
		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();
		//4. ���࿡ �װŸ��� attackRange���� ������
		if (dist < attackRange)
		{
			//5. ����!!! �������
			UE_LOG(LogTemp, Error, TEXT("Attack!!!"));
		}
		else
		{
			//6. �׷��� ������ Idle ���·� ����
			ChangeState(EEnemyState::Idle);
		}
	}
}

void UEnemyFSM::UpdateDamaged()
{
	//damageDelayTime �� ������
	if (IsWaitComplete(damageDelayTime))
	{
		//Move ����
		ChangeState(EEnemyState::Move);
	}
}

void UEnemyFSM::UpdateDie()
{
	//P = P0 + vt
	//1. �Ʒ��� �������� ��ġ�� ���Ѵ�.
	FVector p0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;

	//2. ���࿡ p.Z �� -200 ���� ������ �ı��Ѵ�
	if (p.Z < -200)
	{
		me->Destroy();
	}
	//3. �׷��� ������ �ش� ��ġ�� �����Ѵ�
	else
	{
		me->SetActorLocation(p);
	}

}

void UEnemyFSM::ChangeState(EEnemyState state)
{
	//���� ���� �α׸� �������
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)state));
	}

	//���� ���¸� ����
	currState = state;

	//���¿� ���� �ʱ⼳��
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
	//�Ǹ� ������
	currHP--;
	//hp �� 0���� ũ�� Damage ���·� ��ȯ
	if (currHP > 0)
	{
		ChangeState(EEnemyState::Damaged);
	}
	//�׷��� ������ Die ���·� ��ȯ
	else
	{

		ChangeState(EEnemyState::Die);
	}
}

bool UEnemyFSM::IsWaitComplete(float delayTime)
{
	//1. �ð��� �帣�� �Ѵ�.
	currTime += GetWorld()->DeltaTimeSeconds;
		
	//2. ���࿡ ����ð��� delayTime���� Ŀ���� 
	if (currTime > delayTime)
	{
		//3. ����ð��� 0���� �ʱ�ȭ
		currTime = 0;
		//4. true ��ȯ
		return true;
	}

	//5. �׷��� ������ false �� ��ȯ
	return false;
}
