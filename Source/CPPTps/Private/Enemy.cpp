// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include "EnemyAnim.h"
#include <NavigationInvokerComponent.h>

// Sets default values
AEnemy::AEnemy()
{	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh �� ��ġ�� ����
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	//Mesh �� ȸ���� ����
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	//Mesh ���� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	ConstructorHelpers::FClassFinder<UEnemyAnim> tempAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_EnemyAnim.ABP_EnemyAnim_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	//FSM ������Ʈ �߰�
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	naviInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NAVI"));	

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	
	//LineTrace �� ���ڰ� �ǰ� ����	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//�����̴� �������� ���� �ڵ����� ȸ���϶�� �ɼ�
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}


void AEnemy::SetActive(bool bActive)
{
	if (bActive)
	{		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{		
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);		
		dieDelegate.ExecuteIfBound(this);
	}
	GetMesh()->SetActive(bActive);
	GetMesh()->SetVisibility(bActive);
	GetCharacterMovement()->SetActive(bActive);
	GetCapsuleComponent()->SetActive(bActive);
	fsm->SetActive(bActive);
}
