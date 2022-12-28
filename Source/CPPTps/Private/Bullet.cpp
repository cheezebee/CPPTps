// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/BoxComponent.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Sphere Component 추가
	compSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	SetRootComponent(compSphere);
	compSphere->SetSphereRadius(13);
	//모든것들과 충돌하게 한다
	compSphere->SetCollisionProfileName(TEXT("BlockAll"));

	//StaticMesh Component 추가
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	compMesh->SetupAttachment(RootComponent);
	compMesh->SetRelativeLocation(FVector(0, 0, -13));
	compMesh->SetRelativeScale3D(FVector(0.26f));
	compMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Sphere 외형 가져와서 셋팅하자
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}

	//Projectile Movement Component 셋팅
	compProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE"));
	//Movement 가 움직여야 할 Component를 설정
	compProjectile->SetUpdatedComponent(compSphere);
	//초기속력
	compProjectile->InitialSpeed = 2000;
	//최대속력
	compProjectile->MaxSpeed = 2000;
	//반동여부(튕기는여부)
	compProjectile->bShouldBounce = true;
	//얼마나 잘 튕기게 할것인지 (0 ~ 1)
	compProjectile->Bounciness = 0.5f;

	//생명 시간을 2초로 한다.
	//InitialLifeSpan = 2;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	//Timer 설정하자
	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet::Die, destoryTime, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//앞방향으로 계속 이동하고 싶다
	//P = P0 + vt
	FVector p0 = GetActorLocation();
	FVector vt = GetActorForwardVector() * 2000 * DeltaTime;
	//SetActorLocation(p0 + vt);

	////시간을 누적해준다
	//currTime += DeltaTime;
	////2초가 지나면 
	//if (currTime > destoryTime)
	//{
	//	//나를 파괴하자
	//	Destroy();
	//}
}

void ABullet::Die()
{
	Destroy();
}

