// Fill out your copyright notice in the Description page of Project Settings.


#include "TpsPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include "Enemy.h"
#include "EnemyFSM.h"


// Sets default values
ATpsPlayer::ATpsPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Mesh 위치와 회전을 셋팅
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	//pitch - yaw - roll
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	//외형(Skeletal Mesh) 를 로드해서 셋팅하자
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

	//SkeltalMeshComponent 셋팅 (Rifle)
	compRifle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RIFLE"));
	compRifle->SetupAttachment(GetMesh());
	compRifle->SetRelativeLocation(FVector(-14, 52, 120));

	//SkeltalMesh 불러와서 셋팅
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRifle(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (tempRifle.Succeeded())
	{
		compRifle->SetSkeletalMesh(tempRifle.Object);
	}

	//StaticMeshComponent 셋팅(Sniper)
	compSniper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SNIPER"));
	compSniper->SetupAttachment(GetMesh());
	compSniper->SetRelativeLocation(FVector(-14, 52, 120));
	compSniper->SetRelativeScale3D(FVector(0.15f));

	//StaticMesh 블러와서 셋팅
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	if (tempSniper.Succeeded())
	{
		compSniper->SetStaticMesh(tempSniper.Object);
	}


	//BP_Bullet 클래스 가져오자
	ConstructorHelpers::FClassFinder<ABullet> tempBullet(TEXT("Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (tempBullet.Succeeded())
	{
		bulletFactory = tempBullet.Class;
	}

	//SpnierUI 클래스 가져오자
	ConstructorHelpers::FClassFinder<UUserWidget> tempSniperUI(TEXT("WidgetBlueprint'/Game/Blueprints/SniperUI.SniperUI_C'"));
	if (tempSniperUI.Succeeded())
	{
		sniperUIFactory = tempSniperUI.Class;
	}

	//CommonUI 클래스 가져오자
	ConstructorHelpers::FClassFinder<UUserWidget> tempCommonUI(TEXT("WidgetBlueprint'/Game/Blueprints/CommonUI.CommonUI_C'"));
	if (tempCommonUI.Succeeded())
	{
		commUIFactory = tempCommonUI.Class;
	}

	//폭발 파티클 가져오자
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempExplo(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (tempExplo.Succeeded())
	{
		exploEffect = tempExplo.Object;
	}


	//Controller 의 회전값을 따라 갈 속성 셋팅
	bUseControllerRotationYaw = true;
	compArm->bUsePawnControlRotation = true;

	//점프 횟수를 2개로 하자
	JumpMaxCount = 2;
	//움직이는 속력을 700으로 하자	
	GetCharacterMovement()->MaxWalkSpeed = 700;
	//점프하는 속력을 600으로 하자
	GetCharacterMovement()->JumpZVelocity = 600;
}

// Called when the game starts or when spawned
void ATpsPlayer::BeginPlay()
{
	Super::BeginPlay();

	//카메라 상하 회전값 min, max 셋팅
	APlayerController* playerController = Cast<APlayerController>(Controller);
	playerController->PlayerCameraManager->ViewPitchMin = -45;
	playerController->PlayerCameraManager->ViewPitchMax = 45;

	//처음 총 셋팅을 Sniper 로 하자
	ChangeWeapon(true);

	//SniperUI 만들자
	sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	//CommonUI 만들고 보이게 하자
	commonUI = CreateWidget(GetWorld(), commUIFactory);
	commonUI->AddToViewport();

	//bUseControllerRotationYaw = false;
	//compArm->bUsePawnControlRotation = false;
}

// Called every frame
void ATpsPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//이동
	MoveAction(DeltaTime);
	//회전	
	//RotateAction();
}

void ATpsPlayer::MoveAction(float deltaTime)
{
	//P = P0 + vt
	FVector p0 = GetActorLocation();
	FVector dir = GetActorForwardVector() * v + GetActorRightVector() * h;
	//(1, 0, 0) * v + (0, 1, 0) * h = (v, 0, 0) + (0, h, 0) = (v, h, 0);
	FVector vt = dir.GetSafeNormal() * walkSpeed * deltaTime;
	//SetActorLocation(p0 + vt);

	//Controller 를 이용한 이동
	AddMovementInput(dir.GetSafeNormal());
}

void ATpsPlayer::RotateAction()
{
	//마우스 좌우에 따라서 Actor를 회전 시키자
	SetActorRotation(FRotator(0, mx, 0));
	//마우스 상하에 따라서 SpringArm을 회전 시키자
	compArm->SetRelativeRotation(FRotator(-my, 0, 0));
}

// Called to bind functionality to input
void ATpsPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//A, D
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATpsPlayer::InputHorizontal);
	//W, S
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATpsPlayer::InputVertical);
	//마우스 좌우
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATpsPlayer::InputTurn);
	//마우스 상하
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATpsPlayer::InputLookUp);
	//Space Bar
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ATpsPlayer::InputJump);
	//마우스 왼쪽 버튼
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ATpsPlayer::InputFire);

	
	DECLARE_DELEGATE_OneParam(FChangeWeapon, bool);
	//1번 키
	//PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Released, this, &ATpsPlayer::InputRifle);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Rifle"), IE_Released, this, &ATpsPlayer::ChangeWeapon, false);
	//2번 키
	//PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATpsPlayer::InputSniper);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Sniper"), IE_Released, this, &ATpsPlayer::ChangeWeapon, true);

	//Ctrl 키
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &ATpsPlayer::InputZoomIn);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &ATpsPlayer::InputZoomOut);
}



void ATpsPlayer::InputHorizontal(float value)
{
	h = value;	
}

void ATpsPlayer::InputVertical(float value)
{
	v = value;
}

void ATpsPlayer::InputLookUp(float value)
{
	AddControllerPitchInput(value);
	//my += value;
	//my = FMath::Clamp(my, -45, 45);

	////만약에 my 가 -45 보다 작다면
	//if (my < -45)
	//{
	//	//my 를 -45로 셋팅
	//	my = -45;
	//}
	////만약에 my 가 45 보다 커지면
	//if (my > 45)
	//{
	//	//my 를 45로 셋팅
	//	my = 45;
	//}
}

void ATpsPlayer::InputTurn(float value)
{
	AddControllerYawInput(value);
	//mx += value;
}

void ATpsPlayer::InputJump()
{
	Jump();
}

void ATpsPlayer::InputFire()
{
	//만약에 Rifle 이 보이는 상태라면
	if (compRifle->IsVisible() == true)
	{
		//FirePos Transform 을 가져오자
		FTransform trFire = compRifle->GetSocketTransform(TEXT("FirePos"));

		//총알생성 (위치, 회전 셋팅)
		GetWorld()->SpawnActor<ABullet>(bulletFactory, trFire);
	}
	//Rifle 이 보이지 않는 상태라면 (Sniper 가 보이는 상태)
	else
	{
		//Start, 
		FVector startPos = compCam->GetComponentLocation();
		//End (카메라위치 + 카메라 앞방향 * 거리)
		FVector endPos = startPos + compCam->GetForwardVector() * 5000;
		//충돌이 되었을 때 정보를 담을 변수
		FHitResult hitInfo;
		//충돌 옵션 설정
		FCollisionQueryParams param;
		param.AddIgnoredActor(this);

		//1. Line 을 발사한다.(LineTrace 사용)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, param);
		//2. 만약에 누군가 Line 에 맞았다면
		if (bHit == true)
		{
			//3. 맞은 위치에 폭발 효과를 보여준다.
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				exploEffect,
				hitInfo.ImpactPoint);

			//4. 만약에 맞은애가 isSimulatingPhysics 가 활성화 되어 있다면
			UPrimitiveComponent* compHit = hitInfo.GetComponent();
			if (compHit->IsSimulatingPhysics() == true)
			{
				//5. 맞은지점에 힘을 가한다. 
				//F = m * a;
				FVector force = compHit->GetMass() * compCam->GetForwardVector() * 30000;
				compHit->AddForceAtLocation(force, hitInfo.ImpactPoint);
			}

			//만약에 맞은놈이 Enemy 라면
			AActor* actor = hitInfo.GetActor();
			AEnemy* enemy = Cast<AEnemy>(actor);
			if (enemy != nullptr)
			{
				//Enemy - fsm - ReceiveDamage 함수 호출				
				enemy->fsm->ReceiveDamage();
			}
		}
	}
	
}

void ATpsPlayer::InputRifle()
{
	ChangeWeapon(false);
}

void ATpsPlayer::InputSniper()
{
	ChangeWeapon(true);
}

void ATpsPlayer::ChangeWeapon(bool useSniper)
{
	//만약에 useSniper 가 false (Rifle 로 무기 변경)
	if (useSniper == false && sniperUI->IsInViewport() == true)
	{
		//Common UI를 보이게 한다
		InputZoomOut();
	}

	compRifle->SetVisibility(!useSniper);
	compSniper->SetVisibility(useSniper);

	////2-1 만약에 useSniper 가 true 이면
	//if (useSniper == true)
	//{
	//	//2-2 rifle 을 보이지 않게, sniper 보이게
	//	compRifle->SetVisibility(false);
	//	compSniper->SetVisibility(true);
	//}
	////2-3 그렇지 않다면(useSinper 가 false)
	//else
	//{
	//	//2-4 rifle 을 보이게, sniper 보이지 않게
	//	compRifle->SetVisibility(true);
	//	compSniper->SetVisibility(false);
	//}
}

void ATpsPlayer::InputZoomIn()
{
	//0 만약에 스나이퍼가 보이지 않는 상태라면 함수를 나가겠다.
	if (compSniper->IsVisible() == false) return;

	//1 만들어 놓은 UI ViewPort에 붙이고
	sniperUI->AddToViewport();

	//2 카메라의 Field of View 를 45로
	compCam->SetFieldOfView(45);

	//3 Common UI 를 떼버리자
	commonUI->RemoveFromParent();
}

void ATpsPlayer::InputZoomOut()
{
	//0 만약에 스나이퍼가 보이지 않는 상태라면 함수를 나가겠다.
	if (compSniper->IsVisible() == false) return;

	//1. SniperUI 를 떼버리자
	sniperUI->RemoveFromParent();
	//2. 카메라의 Field of View 를 90로
	compCam->SetFieldOfView(90);

	//3. Common UI 를 ViewPort 에 붙이자
	commonUI->AddToViewport();	

}

