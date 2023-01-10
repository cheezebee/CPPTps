// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Enemy.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	
	createTime = FMath::RandRange(min, max);


	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int32 i = 0; i < 3; i++)
	{		
		AEnemy* e = GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorTransform(), param);
		e->dieDelegate.BindUObject(this, &AEnemyManager::AddEnemy);
		e->SetActive(false);
		//enemyArray.Add(e);
	}
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (enemyArray.Num() > 0)
	{
		currTime += DeltaTime;
		if (currTime > createTime)
		{
			currTime = 0;
			createTime = FMath::RandRange(min, max);

			enemyArray[0]->SetActorLocation(GetActorLocation());
			enemyArray[0]->SetActive(true);
			enemyArray.RemoveAt(0);
			
		}
	}	
}

void AEnemyManager::AddEnemy(AEnemy* e)
{
	enemyArray.Add(e);
}

