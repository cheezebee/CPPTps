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
	Die
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

	//현재시간
	float currTime = 0;
	//대기 상태 기다리는 시간
	float idleDelayTime = 2;

	//공격범위
	float attackRange = 200;


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

	//상태 변경시 한번만!!! 호출 되는 함수
	void ChangeState(EEnemyState state);
};
