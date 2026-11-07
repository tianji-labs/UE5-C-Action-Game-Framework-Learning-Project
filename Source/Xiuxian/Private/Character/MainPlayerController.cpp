// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainPlayerController.h"
#include "HUD/PlayerHeathBarWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	/* ÑªÌõ */
	if (!IsLocalController()||!IsValid(PlayerHealthBarWidgetClass)) return;
	PlayerHealthBarWidget = CreateWidget<UPlayerHeathBarWidget>(this, PlayerHealthBarWidgetClass);

	if (!IsValid(PlayerHealthBarWidget)) return;
	PlayerHealthBarWidget->AddToViewport();
	PlayerHealthBarWidget->SetVisibility(ESlateVisibility::Visible);
	PlayerHealthBarWidget->SetRenderOpacity(1.0f);

}