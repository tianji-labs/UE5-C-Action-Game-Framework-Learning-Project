// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainPlayerController.h"
#include "HUD/PlayerHeathBarWidget.h"
//增强输入
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//插件的组件
#include "Inventory/Public/Player/InventoryInteractionComponent.h"

AMainPlayerController::AMainPlayerController()
{
	InventoryInteractionComponent = CreateDefaultSubobject<UInventoryInteractionComponent>(TEXT("InventoryInteractionComponent"));
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//需要模块"EnhancedInput"
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(IMC_Tianji, 1);
	}

	/* 血条 */
	if (!IsLocalController()||!IsValid(PlayerHealthBarWidgetClass)) return;
	PlayerHealthBarWidget = CreateWidget<UPlayerHeathBarWidget>(this, PlayerHealthBarWidgetClass);

	if (!IsValid(PlayerHealthBarWidget)) return;
	PlayerHealthBarWidget->AddToViewport();
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//绑定插件的输入
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	InventoryInteractionComponent->BindInput(EnhancedInputComponent);
}
