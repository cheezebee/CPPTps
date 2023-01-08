// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

//상태 정의
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damaged,
	Die,
	ReturnPos,
	AttackDelay
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPTPS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	//현재상태
	UPROPERTY(EditAnywhere)
	EEnemyState currState = EEnemyState::Idle;

	//타겟
	UPROPERTY(EditAnywhere)
	class ATpsPlayer* target;
	//나
	UPROPERTY(EditAnywhere)
	class AEnemy* me;

	UPROPERTY(EditAnywhere)
	class UEnemyAnim* anim;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* damagedMontage;

	//현재시간
	float currTime = 0;
	//대기 상태 기다리는 시간
	float idleDelayTime = 2;

	//쫓아 갈 수 있는 범위
	float traceRange = 1000;


	//공격범위
	float attackRange = 200;
	//공격 대기 시간
	float attackDelayTime = 2;

	//피격 대기 시간
	float damageDelayTime = 2;

	//현재 체력
	float currHP;
	//최대 체력
	float maxHP = 3;

	//죽었을 때 내려가는 속력
	float dieSpeed = 100;
	bool bDieMove = false;

	//이동할 수 있는 반경
	float moveRange = 2000;
	//처음 위치를 담아놓을 변수
	FVector originPos;

public:
	//대기
	void UpdateIdle();
	//이동
	void UpdateMove();
	//공격
	void UpdateAttack();
	//피격
	void UpdateDamaged();
	//죽음
	void UpdateDie();
	//리턴
	void UpdateReturnPos();

	void UpdateAttackDelay();

	//상태 변경시 한번만!!! 호출 되는 함수
	void ChangeState(EEnemyState state);

	//공격 받았을 때 처리하는 함수
	void ReceiveDamage();

	//Delay 함수
	bool IsWaitComplete(float delayTime);

	//타겟을 쫓아 갈 수 있니?
	bool IsTargetTrace();
};
