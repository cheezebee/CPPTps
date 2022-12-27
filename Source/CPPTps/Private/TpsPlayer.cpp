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

