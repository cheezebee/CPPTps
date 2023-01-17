// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUI.h"
#include <Components/Button.h>

void UGameOverUI::NativeConstruct()
{
	Super::NativeConstruct();

	btnRetry = Cast<UButton>(GetWidgetFromName(TEXT("_BtnRetry")));
	btnRetry->OnHovered.AddDynamic(this, &UGameOverUI::OnHover);
	btnRetry->OnUnhovered.AddDynamic(this, &UGameOverUI::UnHover);
	
	PlayAnimation(BtnIdle, 0, 0);

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void UGameOverUI::OnHover()
{
	StopAnimation(BtnIdle);
	PlayAnimation(BtnHover, 0, 1);
}

void UGameOverUI::UnHover()
{
	PlayAnimation(BtnIdle, 0, 0);
}