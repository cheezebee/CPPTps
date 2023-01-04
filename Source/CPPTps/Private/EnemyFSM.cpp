// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TpsPlayer.h"
#include <Kismet/GameplayStatics.h>
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
	
	target = Cast<ATpsPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATpsPlayer::StaticClass()));

	me = Cast<AEnemy>(GetOwner());

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
	if (IsWaitComplete(idleDelayTime))
	{
		ChangeState(EEnemyState::Move);
	}
}

void UEnemyFSM::UpdateMove()
{
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	if (dir.Length() > attackRange)
	{
		me->AddMovementInput(dir.GetSafeNormal());
	}
	else
	{
		ChangeState(EEnemyState::Attack);
	}
	
}

void UEnemyFSM::UpdateAttack()
{
	if (IsWaitComplete(attackDelayTime))
	{
		//°Å¸®
		float dist = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
		if (dist < attackRange)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attack!"));
		}
		else
		{
			ChangeState(EEnemyState::Idle);
		}
	}	
}

void UEnemyFSM::UpdateDamaged()
{
	if (IsWaitComplete(damagedDelayTime))
	{
		ChangeState(EEnemyState::Idle);
	}
}

void UEnemyFSM::UpdateDie()
{
	FVector p0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->GetDeltaSeconds();
	FVector p = p0 + vt;

	if (p.Z < -200)
	{
		me->Destroy();
	}
	else
	{
		me->SetActorLocation(p);
	}
}

void UEnemyFSM::ChangeState(EEnemyState state)
{
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -> %s"), *enumPtr->GetNameStringByIndex((int32)currState), *enumPtr->GetNameStringByIndex((int32)state));
	}

	currState = state;
	currTime = 0;
	
	switch (state)
	{
	case EEnemyState::Idle:
		break;
	case EEnemyState::Move:
		break;
	case EEnemyState::Attack:
		currTime = attackDelayTime;
		break;
	case EEnemyState::Damaged:
		break;
	case EEnemyState::Die:
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

bool UEnemyFSM::IsWaitComplete(float delayTime)
{
	currTime += GetWorld()->GetDeltaSeconds();
	if (currTime > delayTime)
	{
		currTime = 0;
		return true;
	}
	return false;
}

void UEnemyFSM::OnDamaged()
{
	currHP--;
	if (currHP > 0)
	{
		ChangeState(EEnemyState::Damaged);
	}
	else
	{
		ChangeState(EEnemyState::Die);
	}
}