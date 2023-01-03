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
	compRifle->SetupAttachment(GetMesh());
	compRifle->SetRelativeLocation(FVector(-14, 52, 120));

	//SkeltalMesh �ҷ��ͼ� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRifle(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (tempRifle.Succeeded())
	{
		compRifle->SetSkeletalMesh(tempRifle.Object);
	}

	//StaticMeshComponent ����(Sniper)
	compSniper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SNIPER"));
	compSniper->SetupAttachment(GetMesh());
	compSniper->SetRelativeLocation(FVector(-14, 52, 120));
	compSniper->SetRelativeScale3D(FVector(0.15f));

	//StaticMesh ���ͼ� ����
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	if (tempSniper.Succeeded())
	{
		compSniper->SetStaticMesh(tempSniper.Object);
	}


	//BP_Bullet Ŭ���� ��������
	ConstructorHelpers::FClassFinder<ABullet> tempBullet(TEXT("Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (tempBullet.Succeeded())
	{
		bulletFactory = tempBullet.Class;
	}

	//SpnierUI Ŭ���� ��������
	ConstructorHelpers::FClassFinder<UUserWidget> tempSniperUI(TEXT("WidgetBlueprint'/Game/Blueprints/SniperUI.SniperUI_C'"));
	if (tempSniperUI.Succeeded())
	{
		sniperUIFactory = tempSniperUI.Class;
	}

	//CommonUI Ŭ���� ��������
	ConstructorHelpers::FClassFinder<UUserWidget> tempCommonUI(TEXT("WidgetBlueprint'/Game/Blueprints/CommonUI.CommonUI_C'"));
	if (tempCommonUI.Succeeded())
	{
		commUIFactory = tempCommonUI.Class;
	}

	//���� ��ƼŬ ��������
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempExplo(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (tempExplo.Succeeded())
	{
		exploEffect = tempExplo.Object;
	}


	//Controller �� ȸ������ ���� �� �Ӽ� ����
	bUseControllerRotationYaw = true;
	compArm->bUsePawnControlRotation = true;

	//���� Ƚ���� 2���� ����
	JumpMaxCount = 2;
	//�����̴� �ӷ��� 700���� ����	
	GetCharacterMovement()->MaxWalkSpeed = 700;
	//�����ϴ� �ӷ��� 600���� ����
	GetCharacterMovement()->JumpZVelocity = 600;
}

// Called when the game starts or when spawned
void ATpsPlayer::BeginPlay()
{
	Super::BeginPlay();

	//ī�޶� ���� ȸ���� min, max ����
	APlayerController* playerController = Cast<APlayerController>(Controller);
	playerController->PlayerCameraManager->ViewPitchMin = -45;
	playerController->PlayerCameraManager->ViewPitchMax = 45;

	//ó�� �� ������ Sniper �� ����
	ChangeWeapon(true);

	//SniperUI ������
	sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	//CommonUI ����� ���̰� ����
	commonUI = CreateWidget(GetWorld(), commUIFactory);
	commonUI->AddToViewport();

	//bUseControllerRotationYaw = false;
	//compArm->bUsePawnControlRotation = false;
}

// Called every frame
void ATpsPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�̵�
	MoveAction(DeltaTime);
	//ȸ��	
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

	//Controller �� �̿��� �̵�
	AddMovementInput(dir.GetSafeNormal());
}

void ATpsPlayer::RotateAction()
{
	//���콺 �¿쿡 ���� Actor�� ȸ�� ��Ű��
	SetActorRotation(FRotator(0, mx, 0));
	//���콺 ���Ͽ� ���� SpringArm�� ȸ�� ��Ű��
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
	//���콺 �¿�
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATpsPlayer::InputTurn);
	//���콺 ����
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATpsPlayer::InputLookUp);
	//Space Bar
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ATpsPlayer::InputJump);
	//���콺 ���� ��ư
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ATpsPlayer::InputFire);

	
	DECLARE_DELEGATE_OneParam(FChangeWeapon, bool);
	//1�� Ű
	//PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Released, this, &ATpsPlayer::InputRifle);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Rifle"), IE_Released, this, &ATpsPlayer::ChangeWeapon, false);
	//2�� Ű
	//PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATpsPlayer::InputSniper);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Sniper"), IE_Released, this, &ATpsPlayer::ChangeWeapon, true);

	//Ctrl Ű
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

	////���࿡ my �� -45 ���� �۴ٸ�
	//if (my < -45)
	//{
	//	//my �� -45�� ����
	//	my = -45;
	//}
	////���࿡ my �� 45 ���� Ŀ����
	//if (my > 45)
	//{
	//	//my �� 45�� ����
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
	//���࿡ Rifle �� ���̴� ���¶��
	if (compRifle->IsVisible() == true)
	{
		//FirePos Transform �� ��������
		FTransform trFire = compRifle->GetSocketTransform(TEXT("FirePos"));

		//�Ѿ˻��� (��ġ, ȸ�� ����)
		GetWorld()->SpawnActor<ABullet>(bulletFactory, trFire);
	}
	//Rifle �� ������ �ʴ� ���¶�� (Sniper �� ���̴� ����)
	else
	{
		//Start, 
		FVector startPos = compCam->GetComponentLocation();
		//End (ī�޶���ġ + ī�޶� �չ��� * �Ÿ�)
		FVector endPos = startPos + compCam->GetForwardVector() * 5000;
		//�浹�� �Ǿ��� �� ������ ���� ����
		FHitResult hitInfo;
		//�浹 �ɼ� ����
		FCollisionQueryParams param;
		param.AddIgnoredActor(this);

		//1. Line �� �߻��Ѵ�.(LineTrace ���)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, param);
		//2. ���࿡ ������ Line �� �¾Ҵٸ�
		if (bHit == true)
		{
			//3. ���� ��ġ�� ���� ȿ���� �����ش�.
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				exploEffect,
				hitInfo.ImpactPoint);

			//4. ���࿡ �����ְ� isSimulatingPhysics �� Ȱ��ȭ �Ǿ� �ִٸ�
			UPrimitiveComponent* compHit = hitInfo.GetComponent();
			if (compHit->IsSimulatingPhysics() == true)
			{
				//5. ���������� ���� ���Ѵ�. 
				//F = m * a;
				FVector force = compHit->GetMass() * compCam->GetForwardVector() * 30000;
				compHit->AddForceAtLocation(force, hitInfo.ImpactPoint);
			}

			//���࿡ �������� Enemy ���
			AActor* actor = hitInfo.GetActor();
			AEnemy* enemy = Cast<AEnemy>(actor);
			if (enemy != nullptr)
			{
				//Enemy - fsm - ReceiveDamage �Լ� ȣ��				
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
	//���࿡ useSniper �� false (Rifle �� ���� ����)
	if (useSniper == false && sniperUI->IsInViewport() == true)
	{
		//Common UI�� ���̰� �Ѵ�
		InputZoomOut();
	}

	compRifle->SetVisibility(!useSniper);
	compSniper->SetVisibility(useSniper);

	////2-1 ���࿡ useSniper �� true �̸�
	//if (useSniper == true)
	//{
	//	//2-2 rifle �� ������ �ʰ�, sniper ���̰�
	//	compRifle->SetVisibility(false);
	//	compSniper->SetVisibility(true);
	//}
	////2-3 �׷��� �ʴٸ�(useSinper �� false)
	//else
	//{
	//	//2-4 rifle �� ���̰�, sniper ������ �ʰ�
	//	compRifle->SetVisibility(true);
	//	compSniper->SetVisibility(false);
	//}
}

void ATpsPlayer::InputZoomIn()
{
	//0 ���࿡ �������۰� ������ �ʴ� ���¶�� �Լ��� �����ڴ�.
	if (compSniper->IsVisible() == false) return;

	//1 ����� ���� UI ViewPort�� ���̰�
	sniperUI->AddToViewport();

	//2 ī�޶��� Field of View �� 45��
	compCam->SetFieldOfView(45);

	//3 Common UI �� ��������
	commonUI->RemoveFromParent();
}

void ATpsPlayer::InputZoomOut()
{
	//0 ���࿡ �������۰� ������ �ʴ� ���¶�� �Լ��� �����ڴ�.
	if (compSniper->IsVisible() == false) return;

	//1. SniperUI �� ��������
	sniperUI->RemoveFromParent();
	//2. ī�޶��� Field of View �� 90��
	compCam->SetFieldOfView(90);

	//3. Common UI �� ViewPort �� ������
	commonUI->AddToViewport();	

}

