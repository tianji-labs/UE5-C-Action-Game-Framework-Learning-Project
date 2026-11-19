// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/HealthBarWidgetComponent.h"
#include "HUD/HealthBarWidget.h"

void UHealthBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	//在敌人头顶显示血条
	if (!IsValid(HealthBarWidgetClass)) return;
	HealthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarWidgetClass);  //根据指定的 Widget 类创建实际的 Widget 对象

	if ( !IsValid(HealthBarWidget)) return;
	SetWidget(HealthBarWidget); 
	SetWidgetSpace(EWidgetSpace::Screen);  
}
