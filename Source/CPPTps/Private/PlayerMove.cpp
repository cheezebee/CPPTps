// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"
#include "TpsPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();	

	//Controller 의 회전값을 따라 갈 속성 셋팅
	me->bUseControllerRotationYaw = true;
	me->compArm->bUsePawnControlRotation = true;

	//점프 횟수를 2개로 하자
	me->JumpMaxCount = 2;
	//움직이는 속력을 walkSpeed 로 하자	
	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	//점프하는 속력을 600으로 하자
	me->GetCharacterMovement()->JumpZVelocity = 600;

	//카메라 상하 회전값 min, max 셋팅
	APlayerController* playerController = Cast<APlayerController>(me->Controller);
	playerController->PlayerCameraManager->ViewPitchMin = -45;
	playerController->PlayerCameraManager->ViewPitchMax = 45;
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//이동
	MoveAction(DeltaTime);
	//회전	
	//RotateAction();
}

void UPlayerMove::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	Super::SetupInputBinding(PlayerInputComponent);

	//A, D
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayerMove::InputHorizontal);
	//W, S
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMove::InputVertical);
	//마우스 좌우
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::InputTurn);
	//마우스 상하
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMove::InputLookUp);
	//Space Bar
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &UPlayerMove::InputJump);
	//Shift 키
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMove::InputRun);
}

void UPlayerMove::MoveAction(float deltaTime)
{
	//P = P0 + vt
	FVector p0 = me->GetActorLocation();
	FVector dir = me->GetActorForwardVector() * v + me->GetActorRightVector() * h;
	//(1, 0, 0) * v + (0, 1, 0) * h = (v, 0, 0) + (0, h, 0) = (v, h, 0);
	FVector vt = dir.GetSafeNormal() * walkSpeed * deltaTime;
	//SetActorLocation(p0 + vt);

	//Controller 를 이용한 이동
	me->AddMovementInput(dir.GetSafeNormal());
}

void UPlayerMove::RotateAction()
{
	//마우스 좌우에 따라서 Actor 를 회전 시키자
	me->SetActorRotation(FRotator(0, mx, 0));
	//마우스 상하에 따라서 SpringArm을 회전 시키자
	me->compArm->SetRelativeRotation(FRotator(-my, 0, 0));
}

void UPlayerMove::InputHorizontal(float value)
{
	h = value;
}

void UPlayerMove::InputVertical(float value)
{
	v = value;
}

void UPlayerMove::InputLookUp(float value)
{
	me->AddControllerPitchInput(value);
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

void UPlayerMove::InputTurn(float value)
{
	me->AddControllerYawInput(value);
	//mx += value;
}

void UPlayerMove::InputJump()
{
	me->Jump();
}

void UPlayerMove::InputRun()
{
	UCharacterMovementComponent* compMove = me->GetCharacterMovement();
	//1. 만약에 MaxWalkSpeed 값이 walkSpeed 보다 크다면(뛰고 있다면)
	if (compMove->MaxWalkSpeed > walkSpeed)
	{
		//2. MaxWalkSpeed 값을 walkSpeed 로 하자
		compMove->MaxWalkSpeed = walkSpeed;
	}
	//3. 그렇지 않으면(걷고 있다면)
	else
	{
		//4. MaxWalkSpeed 값을 runSpeed 로 하자
		compMove->MaxWalkSpeed = runSpeed;
	}
}