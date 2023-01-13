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

	//적을 미리 10개 만들자
	for (int32 i = 0; i < 3; i++)
	{
		//어떤 상황이던 무조건 생성한다는 옵션
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//적 생성
		AEnemy* e = GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), GetActorRotation(), param);		
		//적이 죽었을 때 호출되는 함수 등록
		e->dieDelegate.BindUObject(this, &AEnemyManager::AddEnemy);
		//적을 비활성화 하자
		e->SetActive(false);
	}
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//만약에 적 탄창 갯수가 0이면 함수를 나가자
	if (enemyArray.Num() == 0)
	{
		//애너미를 생성하자
		return;
	}

	//시간을 흐르게 한다
	currTime += DeltaTime;
	//만약에 현재시간이 생성시간보다 커지면
	if (currTime > createTime)
	{
		//어느 위치에 만들지 고른다
		//int32 rand = FMath::RandRange(0, spawnPosArray.Num() - 1);
		//선택된 spawnPos 를 잠시 담아두자
		AActor* spawn = spawnPosArray[spawnIdx];

		//1. 랜덤한 yaw 값 구하자( 0 ~ 360 )
		float yaw = FMath::RandRange(0.0f, 360.0f);
		//2. 구한 값을 spawnPosArray 의 yaw 값에 셋팅
		spawn->SetActorRotation(FRotator(0, yaw, 0));
		//3. 랜덤한 길이 구하자( 0 ~ 500 )
		float length = FMath::RandRange(300.0f, 1000.0f);
		//4. spawnPosArray 앞방향 * 랜덤 길이
		FVector pos = spawn->GetActorForwardVector()* length;
		//5. 4번에서 구한 벡터 + spawnPosArray 의 위치		
				
		//적 위치, 회전 셋팅
		enemyArray[0]->SetActorLocation(spawn->GetActorLocation() + pos);
		enemyArray[0]->SetActorRotation(spawn->GetActorRotation());
		//적 탄창에서 0번째 놈 활성화 
		enemyArray[0]->SetActive(true);
		//enemyArray 에서 빼자
		enemyArray.RemoveAt(0);

		//0 ~ 갯수 순서대로
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

void AEnemyManager::AddEnemy(AEnemy* e)
{
	enemyArray.Add(e);
}
