// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/StartMenu.h"
#include"HUD/MenuWidget.h"

void AStartMenu::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		APlayerController* Controller = World->GetFirstPlayerController();

		checkf(IsValid(MenuWidgetClass), TEXT("MenuWidget is nullptr"));
		if (IsValid(Controller))
		{
			UMenuWidget* MenuWidget = CreateWidget<UMenuWidget>(Controller, MenuWidgetClass);
			MenuWidget->AddToViewport();
		}
	}
}
