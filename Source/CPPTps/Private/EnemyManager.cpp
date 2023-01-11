// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Enemy 공장 셋팅
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
	
	//처음 생성시간 셋팅
	createTime = FMath::RandRange(minTime, maxTime);
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//시간을 흐르게 한다
	currTime += DeltaTime;
	//만약에 현재시간이 생성시간보다 커지면
	if (currTime > createTime)
	{
		//에너미를 만든다
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), GetActorRotation());
		//현재시간 초기화
		currTime = 0;
		//생성시간 새롭게 셋팅
		createTime = FMath::RandRange(minTime, maxTime);
	}
}

