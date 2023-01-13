// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "TpsPlayer.h"
#include "Enemy.h"
#include <Components/CapsuleComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include "EnemyAnim.h"
#include <AIModule/Classes/AIController.h>
#include <NavigationSystem.h>
#include "EnemyManager.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	//SetActive (true / false) 작동 되게 하자!
	bAutoActivate = true;
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	//몽타주 불러오자
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/Blueprints/AMT_EnemyDamage.AMT_EnemyDamage'"));
	if (tempMontage.Succeeded())
	{
		damageMontage = tempMontage.Object;
	}
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//타겟 찾자
	target = Cast<ATpsPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATpsPlayer::StaticClass()));
	//나를 찾자
	me = Cast<AEnemy>(GetOwner());	
	
	//anim instance 찾자
	/*USkeletalMeshComponent* mesh = me->GetMesh();
	UAnimInstance animInstance = mesh->GetAnimInstance();
	anim = Cast<UEnemyAnim>(animInstance);*/
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	//ai controller 찾자
	ai = Cast<AAIController>(me->GetController());
	//ai = UAIBlueprintHelperLibrary::GetAIController(me);

	//나의 초기 체력을 셋팅하자
	currHP = maxHP;

	//나의 초기 위치를 저장하자
	originPos = me->GetActorLocation();

}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (currState)
	{
	case EEnemyState::Idle:
		UpdateIdle();
		break;
	case EEnemyState::Move:
		UpdateMove();
		break;
	case EEnemyState::Attack:
		UpdateAttack();
		break;
	case EEnemyState::AttackDelay:
		UpdaetAttackDelay();
		break;
	case EEnemyState::Damaged:
		UpdateDamaged();
		break;
	case EEnemyState::Die:
		UpdateDie();
		break;
	case EEnemyState::ReturnPos:
		UpdateReturnPos();
		break;
	}
}

void UEnemyFSM::UpdateIdle()
{
	//만약에 플레이어를 쫓아 갈 수 있니?
	if (IsTargetTrace())
	{
		//상태를 Move 로 전환
		ChangeState(EEnemyState::Move);
	}
	else
	{
		//idleDelayTime 이 지나면	
		if (IsWaitComplete(idleDelayTime))
		{
			//현재상태를 Move 로 한다.
			ChangeState(EEnemyState::Move);
		}
	}
}

void UEnemyFSM::UpdateMove()
{
	// 시야에 들어왔는지 여부
	bool bTrace = IsTargetTrace();

	//1. 타겟을 향하는 방향을 구하고(target - me)
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//처음 위치, 나의 현재 위치의 거리
	float dist = FVector::Distance(originPos, me->GetActorLocation());
	
	//만약에 dist 가 moveRange 보다 커지면 (움직일 수 있는 반경을 넘어갔다면)
	if (dist > moveRange)
	{
		//상태를 ReturnPos 로 변경
		ChangeState(EEnemyState::ReturnPos);
	}
	//만약에 시야에 들어왔다면
	else if (bTrace)
	{
		//만약에 target - me 거리가 공격범위보다 작으면
		if (dir.Length() < attackRange)
		{
			//상태를 Attack 으로 변경
			ChangeState(EEnemyState::Attack);
		}
		//그렇지 않으면
		else
		{
			//2. 그 방향으로 이동하고 싶다.
			//me->AddMovementInput(dir.GetSafeNormal());
			//ai 를 이용해서 목적지까지 이동하고 싶다.	
			ai->MoveToLocation(target->GetActorLocation());
		}
	}
	//시야에 들어오지 않았다면
	else
	{
		// 랜덤한 위치까지 도착한 후 Idle 상태로 전환
		MoveToPos(randPos);
	}
	
}

void UEnemyFSM::UpdateAttack()
{	
	//2. 상태를 AttackDelay 로 전환
	ChangeState(EEnemyState::AttackDelay);	
}

void UEnemyFSM::UpdaetAttackDelay()
{
	//2. 만약에 현재시간이 attackDelayTime 보다 커지면
	if (IsWaitComplete(attackDelayTime))
	{
		//3. target 과 me 거리를 구하자.
		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();
		//4. 만약에 그거리가 attackRange보다 작으면
		if (dist < attackRange)
		{
			//5. Attack 상태로 전환
			ChangeState(EEnemyState::Attack);
		}
		else
		{
			//6. 그렇지 않으면 Idle 상태로 가자
			ChangeState(EEnemyState::Idle);
		}
	}
}

void UEnemyFSM::UpdateDamaged()
{
	//damageDelayTime 이 지나면
	if (IsWaitComplete(damageDelayTime))
	{
		//Move 상태
		ChangeState(EEnemyState::Idle);
	}
}

void UEnemyFSM::UpdateDie()
{
	//만약에 bDieMove 가 false 라면 함수를 나가라
	if (bDieMove == false) return;

	//P = P0 + vt
	//1. 아래로 내려가는 위치를 구한다.
	FVector p0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;

	//2. 만약에 p.Z 가 -200 보다 작으면 파괴한다
	if (p.Z < -200)
	{
		//me->Destroy();
		
		//나를 비활성화
		me->SetActive(false);
		////EnemyManager 찾자
		//AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyManager::StaticClass());
		//AEnemyManager* am = Cast<AEnemyManager>(actor);
		////찾은 놈에서 enemyArray 에 나를 다시 담자
		//am->enemyArray.Add(me);
		//currHP 를 maxHP
		currHP = maxHP;
		//상태를 Idle
		ChangeState(EEnemyState::Idle);
		//몽타주를 멈춰준다
		me->StopAnimMontage(damageMontage);
		//bDieMove 를 false 로!
		bDieMove = false;
	}
	//3. 그렇지 않으면 해당 위치로 셋팅한다
	else
	{
		me->SetActorLocation(p);
	}
}

void UEnemyFSM::UpdateReturnPos()
{
	// 처음 위치로 가서 도착하면 Idle 상태로 전환한다.
	MoveToPos(originPos);
	

	////1. 나 ----> 처음위치를 향하는 방향을 구한다.
	//FVector dir = originPos - me->GetActorLocation();

	////2. 만약에 나 --- 처음위치의 거리가 10보다 작으면
	//if (dir.Length() < 10)
	//{
	//	//3. Idle 상태로 전환
	//	ChangeState(EEnemyState::Idle);
	//}
	////4. 그렇지 않으면 
	//else
	//{
	//	//5. 계속 1번에서 구한 방향으로 이동한다
	//	me->AddMovementInput(dir.GetSafeNormal());
	//}
}

void UEnemyFSM::ChangeState(EEnemyState state)
{
	//상태 변경 로그를 출력하자
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)state));
	}		

	//현재 상태를 갱신
	currState = state;

	//anim 의 상태 갱신
	anim->state = state;

	//현재 시간 초기화
	currTime = 0;	

	//ai 의 움직임 멈추자
	ai->StopMovement();

	//상태에 따른 초기설정
	switch (currState)
	{
	case EEnemyState::Attack:
		//currTime = attackDelayTime;
		break;
	case EEnemyState::Move:
	{
		//네비게이션 시스템 가져오자
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//랜덤한 위치를 얻오자
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
		break;
	case EEnemyState::Damaged:
	{
		//1. 랜덤한 값을 뽑는다 (0, 1 중)
		int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 이란 문자열을 만든다.
		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. 몽타주를 플레이한다.
		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
		break;
	case EEnemyState::Die:		
		//충돌안되게 설정
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Die 몽타주 실행
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));
		break;
	}
}

void UEnemyFSM::ReceiveDamage()
{
	//피를 줄이자
	currHP--;
	//hp 가 0보다 크면 Damage 상태로 전환
	if (currHP > 0)
	{
		ChangeState(EEnemyState::Damaged);
	}
	//그렇지 않으면 Die 상태로 전환
	else
	{

		ChangeState(EEnemyState::Die);
	}
}

bool UEnemyFSM::IsWaitComplete(float delayTime)
{
	//1. 시간을 흐르게 한다.
	currTime += GetWorld()->DeltaTimeSeconds;
		
	//2. 만약에 현재시간이 delayTime보다 커지면 
	if (currTime > delayTime)
	{
		//3. 현재시간을 0으로 초기화
		currTime = 0;
		//4. true 반환
		return true;
	}

	//5. 그렇지 않으면 false 를 반환
	return false;
}


bool UEnemyFSM::IsTargetTrace()
{
	//1. 나 ----> 플레이어를 향하는 벡터
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//2. 나의 앞방향과 1번에 구한 벡터의 내적
	float dotValue = FVector::DotProduct(me->GetActorForwardVector(), dir.GetSafeNormal());

	//3. 2번에 구한 값을 Acos --> 두 벡터의 사이각
	float angle = UKismetMathLibrary::DegAcos(dotValue);
	
	//4. 만약에 3번에서 구한 값이 30보다 작고(시야각 60)
	//   나 - 타겟 과의 거리가 traceRange 보다 작으면
	if (angle < 30 && dir.Length() < traceRange)
	{
		//Enemy -----> target LineTrace 쏘자!!
		FHitResult hitInfo;
		FCollisionQueryParams param;
		param.AddIgnoredActor(me);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			hitInfo,
			me->GetActorLocation(),
			target->GetActorLocation(),
			ECollisionChannel::ECC_Visibility,
			param);

		//만약에 부딪힌 곳이 있다면
		if (bHit)
		{
			//만약에 부딪힌 놈의 이름이 Player 를 포함하고 있다면
			if (hitInfo.GetActor()->GetName().Contains(TEXT("Player")))
			{
				//5. true 반환
				return true;
			}	
		}
	}

	//6. 그렇지 않으면 false 반환	
	return false;

}

void UEnemyFSM::MoveToPos(FVector pos)
{
	//해당 위치(pos) 로 간다
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//만약에 목적지에 도착했다면
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle 상태로 전환
		ChangeState(EEnemyState::Idle);
	}
}
