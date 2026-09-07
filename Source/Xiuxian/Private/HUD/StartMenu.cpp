// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/StartMenu.h"
#include"HUD/MenuWidget.h"
#include"Character/Tianji.h"
#include"Kismet/GameplayStatics.h"

void AStartMenu::BeginPlay()
{
	Super::BeginPlay();

	//动态多播委托
	ATianji* PC = CastChecked<ATianji>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(PC))
	{
		PC->OnOpenMenuRequest.AddDynamic(this, &AStartMenu::OpenMenu);
	}
	//开始显示菜单
	OpenMenu();
}

//解绑
void AStartMenu::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ATianji* PlayerChar = Cast<ATianji>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(PlayerChar))
	{
		//把this(HUD实例)从委托绑定列表移除
		PlayerChar->OnOpenMenuRequest.RemoveAll(this);
	}
}
//打开菜单
void AStartMenu::OpenMenu()
{
	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		APlayerController* Controller = World->GetFirstPlayerController();

		if (IsValid(Controller))
		{
			Controller->SetPause(true);
			UMenuWidget* MenuWidget = CreateWidget<UMenuWidget>(Controller, MenuWidgetClass);
			MenuWidget->AddToViewport();
		}
	}
}
