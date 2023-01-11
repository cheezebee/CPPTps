// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>

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

	//SpawnPos ����
	FindSpawnPos();
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
		//��� ��ġ�� ������ ����
		//int32 rand = FMath::RandRange(0, spawnPosArray.Num() - 1);
		//0 ~ ���� �������
		
		//���ʹ̸� �����
		GetWorld()->SpawnActor<AEnemy>(
			enemyFactory, 
			spawnPosArray[spawnIdx]->GetActorLocation(),
			spawnPosArray[spawnIdx]->GetActorRotation());

		//���� �ڸ��� ����
		spawnIdx++;
		spawnIdx = spawnIdx % spawnPosArray.Num();

		////���࿡ spawnIdx �� spawnPosArray ������ ��������
		//if (spawnIdx == spawnPosArray.Num())
		//{
		//	//spawnIdx �� 0 ���� �Ѵ�.
		//	spawnIdx = 0;
		//}

		//����ð� �ʱ�ȭ
		currTime = 0;
		//�����ð� ���Ӱ� ����
		createTime = FMath::RandRange(minTime, maxTime);
	}
}


void AEnemyManager::FindSpawnPos()
{
	//���� ���͸� ã�Ƽ� ��������
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), actors);

	for (int32 i = 0; i < actors.Num(); i++)
	{
		//���࿡ �̸��� SpawnPos �� ���ԵǾ� �ִٸ�
		if (actors[i]->GetName().Contains(TEXT("SpawnPos")))
		{
			
			//spawnPosArray �� �߰��Ѵ�
			spawnPosArray.Add(actors[i]);
		}
	}
}
