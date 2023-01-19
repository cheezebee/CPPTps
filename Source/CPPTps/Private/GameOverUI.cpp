// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUI.h"
#include <Kismet/GameplayStatics.h>
#include <Components/Button.h>

void UGameOverUI::NativeConstruct()
{
	Super::NativeConstruct();

	//마우스 보이게 하자
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	//게임 일시 정지
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	//버튼 이벤트에 호출되는 함수 등록
	BtnRetry->OnHovered.AddDynamic(this, &UGameOverUI::OnHover);
}

void UGameOverUI::OnHover()
{
	UE_LOG(LogTemp, Warning, TEXT("OnHover"));
}

void UGameOverUI::UnHover()
{

}

void UGameOverUI::OnClick()
{

}