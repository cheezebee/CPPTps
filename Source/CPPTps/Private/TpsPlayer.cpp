// Fill out your copyright notice in the Description page of Project Settings.


#include "TpsPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

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

