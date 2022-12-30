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
	//�������
	UPROPERTY(EditAnywhere)
	EEnemyState currState = EEnemyState::Idle;

	//����ð�
	float currTime = 0;
	//��� ���� ��ٸ��� �ð�
	float idleDelayTime = 2;


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
};
