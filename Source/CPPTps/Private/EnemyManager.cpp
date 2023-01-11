// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>

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

	//SpawnPos 셋팅
	FindSpawnPos();
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
		//어느 위치에 만들지 고른다
		//int32 rand = FMath::RandRange(0, spawnPosArray.Num() - 1);
		//0 ~ 갯수 순서대로
		
		//에너미를 만든다
		GetWorld()->SpawnActor<AEnemy>(
			enemyFactory, 
			spawnPosArray[spawnIdx]->GetActorLocation(),
			spawnPosArray[spawnIdx]->GetActorRotation());

		//다음 자리로 설정
		spawnIdx++;
		spawnIdx = spawnIdx % spawnPosArray.Num();

		////만약에 spawnIdx 가 spawnPosArray 갯수랑 같아지면
		//if (spawnIdx == spawnPosArray.Num())
		//{
		//	//spawnIdx 을 0 으로 한다.
		//	spawnIdx = 0;
		//}

		//현재시간 초기화
		currTime = 0;
		//생성시간 새롭게 셋팅
		createTime = FMath::RandRange(minTime, maxTime);
	}
}


void AEnemyManager::FindSpawnPos()
{
	//스폰 액터를 찾아서 가져오자
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actors);

	for (int32 i = 0; i < actors.Num(); i++)
	{
		//만약에 이름에 SpawnPos 가 포함되어 있다면
		if (actors[i]->GetName().Contains(TEXT("SpawnPos")))
		{
			
			//spawnPosArray 에 추가한다
			spawnPosArray.Add(actors[i]);
		}
	}
}
