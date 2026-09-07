// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MenuWidget.h"
//按钮
#include"Components/Button.h"
//
#include "Kismet/GameplayStatics.h"
#include"GameFramework/PlayerController.h"
//退游戏
#include "Kismet/KismetSystemLibrary.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//按钮绑函数
	if (IsValid(Start))
	{
		Start->OnClicked.AddDynamic(this, &UMenuWidget::StartGame);
	}
	if (IsValid(Start))
	{
		End->OnClicked.AddDynamic(this, &UMenuWidget::EndGame);
	}
	//显示鼠标
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC))
	{
		PC->bShowMouseCursor = true;
		FInputModeUIOnly UIMode;
		PC->SetInputMode(UIMode);
	}
}

void UMenuWidget::StartGame()
{
	SetVisibility(ESlateVisibility::Hidden);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC))
	{
		PC->SetPause(false);
		PC->bShowMouseCursor = false;
		FInputModeGameOnly GameMode;
		PC->SetInputMode(GameMode);
	}
}

void UMenuWidget::EndGame()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, false);
	}
}
