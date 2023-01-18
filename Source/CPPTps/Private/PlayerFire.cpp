// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFire.h"
#include "Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Particles/ParticleSystem.h>
#include "ABP_Player.h"
#include "TpsPlayer.h"
#include <Camera/CameraComponent.h>
#include <CollisionQueryParams.h>
#include <Kismet/GameplayStatics.h>
#include "Enemy.h"
#include "EnemyFSM.h"
#include "MainUI.h"

UPlayerFire::UPlayerFire()
{	
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
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	//SniperUI 만들자
	sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	//CommonUI 만들고 보이게 하자
	commonUI = CreateWidget(GetWorld(), commUIFactory);
	commonUI->AddToViewport();

	//처음 총 셋팅을 Sniper 로 하자
	ChangeWeapon(true);
}

void UPlayerFire::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//1. 만약에 bFire 가 true 라면
	if (bFire == true)
	{
		//2. 현재시간을 흐르게 하고
		currCamShakeTime += DeltaTime;
		//3. 만약에 현재시간이 기준시간보다 작으면
		if (currCamShakeTime < camShakeTime)
		{			
			//4. 카메라를 랜덤하게 위치시키자
			float randY = FMath::RandRange(-5.0f, 5.0f);
			float randZ = FMath::RandRange(-5.0f, 5.0f);
			me->compCam->SetRelativeLocation(FVector(0, randY, randZ));
		}
		//5. 그렇지 않으면 초기화(현재시간, bFire, 카메라위치)
		else
		{
			currCamShakeTime = 0;
			bFire = false;
			me->compCam->SetRelativeLocation(FVector::ZeroVector);
		}
	}
}

void UPlayerFire::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	Super::SetupInputBinding(PlayerInputComponent);

	//마우스 왼쪽 버튼
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &UPlayerFire::InputFire);


	DECLARE_DELEGATE_OneParam(FChangeWeapon, bool);
	//1번 키
	//PlayerInputComponent->BindAction(TEXT("Rifle"), IE_Released, this, &ATpsPlayer::InputRifle);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Rifle"), IE_Released, this, &UPlayerFire::ChangeWeapon, false);
	//2번 키
	//PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATpsPlayer::InputSniper);
	PlayerInputComponent->BindAction<FChangeWeapon>(TEXT("Sniper"), IE_Released, this, &UPlayerFire::ChangeWeapon, true);

	//Ctrl 키
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &UPlayerFire::InputZoomIn);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &UPlayerFire::InputZoomOut);
}

void UPlayerFire::InputFire()
{
	//APlayerController* controller = GetWorld()->GetFirstPlayerController();
	////카메라 흔들림을 멈추자
	//controller->PlayerCameraManager->StopAllCameraShakes();
	////카메라를 흔들자
	//controller->PlayerCameraManager->StartCameraShake(cameraShake);

	//총을 쐈다!!
	bFire = true;


	//총쏘는 애니메이션을 하자
	UABP_Player* playerAnim = Cast<UABP_Player>(me->GetMesh()->GetAnimInstance());
	playerAnim->PlayAttackAnim();

	//만약에 Rifle 이 보이는 상태라면
	if (me->compRifle->IsVisible() == true)
	{
		//FirePos Transform 을 가져오자
		FTransform trFire = me->compRifle->GetSocketTransform(TEXT("FirePos"));

		//총알생성 (위치, 회전 셋팅)
		GetWorld()->SpawnActor<ABullet>(bulletFactory, trFire);
	}
	//Rifle 이 보이지 않는 상태라면 (Sniper 가 보이는 상태)
	else
	{
		//Start, 
		FVector startPos = me->compCam->GetComponentLocation();
		//End (카메라위치 + 카메라 앞방향 * 거리)
		FVector endPos = startPos + me->compCam->GetForwardVector() * 5000;
		//충돌이 되었을 때 정보를 담을 변수
		FHitResult hitInfo;
		//충돌 옵션 설정
		FCollisionQueryParams param;
		param.AddIgnoredActor(me);

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
				FVector force = compHit->GetMass() * me->compCam->GetForwardVector() * 30000;
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

void UPlayerFire::InputRifle()
{
	ChangeWeapon(false);
}

void UPlayerFire::InputSniper()
{
	ChangeWeapon(true);
}



void UPlayerFire::ChangeWeapon(bool useSniper)
{
	//만약에 useSniper 가 false (Rifle 로 무기 변경)
	if (useSniper == false && sniperUI->IsInViewport() == true)
	{
		//Common UI를 보이게 한다
		InputZoomOut();
	}

	me->compRifle->SetVisibility(!useSniper);
	me->compSniper->SetVisibility(useSniper);

	//무기 UI 갱신
	//if (me->mainUI != nullptr)
	{
		me->mainUI->UpdateWeapon(useSniper);
	}

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

void UPlayerFire::InputZoomIn()
{
	//0 만약에 스나이퍼가 보이지 않는 상태라면 함수를 나가겠다.
	if (me->compSniper->IsVisible() == false) return;

	//1 만들어 놓은 UI ViewPort에 붙이고
	sniperUI->AddToViewport();

	//2 카메라의 Field of View 를 45로
	me->compCam->SetFieldOfView(45);

	//3 Common UI 를 떼버리자
	commonUI->RemoveFromParent();
}

void UPlayerFire::InputZoomOut()
{
	//0 만약에 스나이퍼가 보이지 않는 상태라면 함수를 나가겠다.
	if (me->compSniper->IsVisible() == false) return;

	//1. SniperUI 를 떼버리자
	sniperUI->RemoveFromParent();
	//2. 카메라의 Field of View 를 90로
	me->compCam->SetFieldOfView(90);

	//3. Common UI 를 ViewPort 에 붙이자
	commonUI->AddToViewport();

}
