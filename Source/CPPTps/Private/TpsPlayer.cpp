// Fill out your copyright notice in the Description page of Project Settings.


#include "TpsPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include "ABP_Player.h"
#include "PlayerMove.h"
#include "PlayerFire.h"
#include "MainUI.h"
#include "MiniMap.h"
#include <Engine/EngineTypes.h>


// Sets default values
ATpsPlayer::ATpsPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh ��ġ�� ȸ���� ����
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	//pitch - yaw - roll
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	//����(Skeletal Mesh) �� �ε��ؼ� ��������
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	//SpringArmComponent
	compArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ARM"));
	compArm->SetupAttachment(RootComponent);
	compArm->SetRelativeLocation(FVector(0, 70, 90));
	compArm->TargetArmLength = 300;

	//CameraComponent
	compCam = CreateDefaultSubobject<UCameraComponent>(TEXT("CAM"));
	compCam->SetupAttachment(compArm);

	//SkeltalMeshComponent ���� (Rifle)
	compRifle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RIFLE"));
	compRifle->SetupAttachment(GetMesh(), TEXT("WeaponPos"));
	compRifle->SetRelativeLocation(FVector(-17, 10, -3));
	compRifle->SetRelativeRotation(FRotator(0, 90, 0));

	//SkeltalMesh �ҷ��ͼ� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRifle(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (tempRifle.Succeeded())
	{
		compRifle->SetSkeletalMesh(tempRifle.Object);
	}

	//StaticMeshComponent ����(Sniper)
	compSniper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SNIPER"));
	compSniper->SetupAttachment(GetMesh(), TEXT("WeaponPos"));
	
	compSniper->SetRelativeLocation(FVector(-42, 7, 1));
	compSniper->SetRelativeRotation(FRotator(0, 90, 0));
	compSniper->SetRelativeScale3D(FVector(0.15f));

	//StaticMesh ���ͼ� ����
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	if (tempSniper.Succeeded())
	{
		compSniper->SetStaticMesh(tempSniper.Object);
	}


	//�ִϸ��̼� �������Ʈ ��������
	ConstructorHelpers::FClassFinder<UABP_Player> tempAnim(TEXT("AnimBlueprint'/Game/Blueprints/ABP_Player.ABP_Player_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PM"));
	playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PF"));

	ConstructorHelpers::FClassFinder<UMainUI> tempMain(TEXT("WidgetBlueprint'/Game/Blueprints/BP_MainUI.BP_MainUI_C'"));
	if (tempMain.Succeeded())
	{
		mainUIFactory = tempMain.Class;
	}

	mainUI = CreateWidget<UMainUI>(GetWorld(), mainUIFactory);

	ConstructorHelpers::FClassFinder<AMiniMap> tempMiniMap(TEXT("Blueprint'/Game/Blueprints/BP_Minimap.BP_Minimap_C'"));
	if (tempMiniMap.Succeeded())
	{
		miniMapFactory = tempMiniMap.Class;
	}

	//Camera Shake �������Ʈ ��������
	/*ConstructorHelpers::FClassFinder<UCameraShakeBase> tempCam(TEXT("Blueprint'/Game/Blueprints/BP_CameraShake.BP_CameraShake_C'"));
	if (tempCam.Succeeded())
	{
		cameraShake = tempCam.Class;
	}*/		
}

// Called when the game starts or when spawned
void ATpsPlayer::BeginPlay()
{

	Super::BeginPlay();
	
	currHP = maxHP;
	mainUI->AddToViewport();
	//CapsuleCompoenent �� ECC_Visibility -> ECR_Block �� ����
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AMiniMap* miniMap = GetWorld()->SpawnActor<AMiniMap>(miniMapFactory);
	miniMap->Init(this);
	//miniMap->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called every frame
void ATpsPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	//1. ���࿡ bFire �� true ���
	if (bFire == true)
	{
		//2. ����ð��� �帣�� �ϰ�
		currCamShakeTime += DeltaTime;
		//3. ���࿡ ����ð��� ���ؽð����� ������
		if (currCamShakeTime < camShakeTime)
		{
			//4. ī�޶� �����ϰ� ��ġ��Ű��
			float randY = FMath::RandRange(-5.0f, 5.0f);
			float randZ = FMath::RandRange(-5.0f, 5.0f);
			compCam->SetRelativeLocation(FVector(0, randY, randZ));
		}
		//5. �׷��� ������ �ʱ�ȭ(����ð�, bFire, ī�޶���ġ)
		else
		{
			currCamShakeTime = 0;
			bFire = false;
			compCam->SetRelativeLocation(FVector::ZeroVector);
		}
	}
}



// Called to bind functionality to input
void ATpsPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	playerMove->SetupInputBinding(PlayerInputComponent);
	playerFire->SetupInputBinding(PlayerInputComponent);
}

void ATpsPlayer::OnHit(float damage)
{
	currHP -= damage;


	if (currHP <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Die!!"));
	}
	else
	{
		mainUI->UpdateCurrHP(currHP, maxHP);
		UE_LOG(LogTemp, Warning, TEXT("OnHit!!"));
	}
}




