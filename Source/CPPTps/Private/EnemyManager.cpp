// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Enemy ���� ����
	ConstructorHelpers::FClassFinder<AEnemy> tempEnemy(TEXT("Blueprint'/Game/Blueprints/BP_Enemy.BP_Enemy_C'"));
	if (tempEnemy.Succeeded())
	{
		enemyFactory = tempEnemy.Class;
	}
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	//ó�� �����ð� ����
	createTime = FMath::RandRange(minTime, maxTime);
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�ð��� �帣�� �Ѵ�
	currTime += DeltaTime;
	//���࿡ ����ð��� �����ð����� Ŀ����
	if (currTime > createTime)
	{
		//���ʹ̸� �����
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), GetActorRotation());
		//����ð� �ʱ�ȭ
		currTime = 0;
		//�����ð� ���Ӱ� ����
		createTime = FMath::RandRange(minTime, maxTime);
	}
}

