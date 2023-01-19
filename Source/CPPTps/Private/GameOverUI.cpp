// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUI.h"
#include <Kismet/GameplayStatics.h>
#include <Components/Button.h>

void UGameOverUI::NativeConstruct()
{
	Super::NativeConstruct();

	//���콺 ���̰� ����
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	//���� �Ͻ� ����
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	//��ư �̺�Ʈ�� ȣ��Ǵ� �Լ� ���
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