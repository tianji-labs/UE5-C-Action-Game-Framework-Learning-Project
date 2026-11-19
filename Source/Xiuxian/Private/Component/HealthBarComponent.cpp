// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/HealthBarComponent.h"
//控件组件
#include "Component/HealthBar.h"
//控件
#include "HUD/HealthBarWidget.h"
//进度条
#include"Components/ProgressBar.h"
//属性组件
#include "Component/AttributeComponent.h"


UHealthBarComponent::UHealthBarComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}


void UHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (IsValid(Owner))
	{
		AttributeComponent = Owner->FindComponentByClass<UAttributeComponent>(); 
		HealthBar = Owner->FindComponentByClass<UHealthBar>(); //查询引用自身拥有的组件

	}

	CreateHealthBarWidget(); //先初始化HealthBar

	if(AttributeComponent.IsValid())
	{
		AttributeComponent->OnHealthChanged.AddDynamic(this, &UHealthBarComponent::HandleHealthBar); //绑定血量变化委托
	}

	if (!IsValid(HealthBar)) return;
	HealthBar->SetVisibility(false);

}

/* 血条 */
void UHealthBarComponent::CreateHealthBarWidget()
{
	if (!IsValid(HealthBarWidgetClass)) return;
	HealthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(),HealthBarWidgetClass);  //根据指定的 Widget 类创建实际的 Widget 对象

	if (!IsValid(HealthBar) || !IsValid(HealthBarWidget)) return;
	HealthBar->SetWidget(HealthBarWidget);  // UHealthBar 负责把 Widget 放到游戏世界中显示
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
}

void UHealthBarComponent::HandleHealthBar(float NewHealthPercent)
{
	SetHealthBarPercent(NewHealthPercent);
	if (!AttributeComponent.IsValid()) return;
	if (AttributeComponent->IsDead())
	{
		HealthBar->SetVisibility(false);
	}
	else
	{
		HealthBar->SetVisibility(true);
	}

}

void UHealthBarComponent::SetHealthBarPercent(float Percent)
{
	if (!IsValid(HealthBarWidget) || !IsValid(HealthBarWidget->HealthBar)) return;
	HealthBarWidget->HealthBar->SetPercent(Percent);
}

