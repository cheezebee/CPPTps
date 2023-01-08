// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

//���� ����
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
	//�������
	UPROPERTY(EditAnywhere)
	EEnemyState currState = EEnemyState::Idle;

	//Ÿ��
	UPROPERTY(EditAnywhere)
	class ATpsPlayer* target;
	//��
	UPROPERTY(EditAnywhere)
	class AEnemy* me;

	UPROPERTY(EditAnywhere)
	class UEnemyAnim* anim;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* damagedMontage;

	//����ð�
	float currTime = 0;
	//��� ���� ��ٸ��� �ð�
	float idleDelayTime = 2;

	//�Ѿ� �� �� �ִ� ����
	float traceRange = 1000;


	//���ݹ���
	float attackRange = 200;
	//���� ��� �ð�
	float attackDelayTime = 2;

	//�ǰ� ��� �ð�
	float damageDelayTime = 2;

	//���� ü��
	float currHP;
	//�ִ� ü��
	float maxHP = 3;

	//�׾��� �� �������� �ӷ�
	float dieSpeed = 100;
	bool bDieMove = false;

	//�̵��� �� �ִ� �ݰ�
	float moveRange = 2000;
	//ó�� ��ġ�� ��Ƴ��� ����
	FVector originPos;

public:
	//���
	void UpdateIdle();
	//�̵�
	void UpdateMove();
	//����
	void UpdateAttack();
	//�ǰ�
	void UpdateDamaged();
	//����
	void UpdateDie();
	//����
	void UpdateReturnPos();

	void UpdateAttackDelay();

	//���� ����� �ѹ���!!! ȣ�� �Ǵ� �Լ�
	void ChangeState(EEnemyState state);

	//���� �޾��� �� ó���ϴ� �Լ�
	void ReceiveDamage();

	//Delay �Լ�
	bool IsWaitComplete(float delayTime);

	//Ÿ���� �Ѿ� �� �� �ִ�?
	bool IsTargetTrace();
};
