// Fill out your copyright notice in the Description page of Project Settings.


#include "TpsPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h>


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

}

// Called when the game starts or when spawned
void ATpsPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATpsPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//P = P0 + vt
	FVector p0 = GetActorLocation();
	FVector dir = GetActorForwardVector() * v + GetActorRightVector() * h; 
	            //(1, 0, 0) * v + (0, 1, 0) * h = (v, 0, 0) + (0, h, 0) = (v, h, 0);
	FVector vt = dir.GetSafeNormal() * walkSpeed * DeltaTime;
	//SetActorLocation(p0 + vt);

	//Controller �� �̿��� �̵�
	AddMovementInput(dir.GetSafeNormal());
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
}

void ATpsPlayer::InputTurn(float value)
{
	AddControllerYawInput(value);
}

