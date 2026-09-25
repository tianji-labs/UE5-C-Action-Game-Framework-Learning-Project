// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/HealthBarComponent.h"
//控件组件
#include "Component/HealthBar.h"
//控件
#include "HUD/HealthBarWidget.h"
//进度条
#include"Components/ProgressBar.h"


UHealthBarComponent::UHealthBarComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}


void UHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	HealthBar = Owner->FindComponentByClass<UHealthBar>(); //查询引用自身拥有的组件

	CreateHealthBarWidget(); //先初始化HealthBar
	CurrentHealthyBar = MaxHealthyBar;

	if (!IsValid(Owner)) return;
	Owner->OnTakeAnyDamage.AddDynamic(this,&UHealthBarComponent::TakeAnyDamage);

	if (!IsValid(HealthBar)) return;
	HealthBar->SetVisibility(false);

}

void UHealthBarComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//不要return可能跳过销毁
	if (IsValid(GetOwner()))
	{
		GetOwner()->OnTakeAnyDamage.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

/* 血条UI */
void UHealthBarComponent::CreateHealthBarWidget()
{
	if (!IsValid(HealthBarWidgetClass)) return;
	HealthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(),HealthBarWidgetClass);  //根据指定的 Widget 类创建实际的 Widget 对象

	if (!IsValid(HealthBar) || !IsValid(HealthBarWidget)) return;
	HealthBar->SetWidget(HealthBarWidget);  // UHealthBar 负责把 Widget 放到游戏世界中显示
	HealthBar->SetWidgetSpace(EWidgetSpace::Screen);
}

/* 血条百分比 */
float UHealthBarComponent::GetHealthyPercent()
{
	return CurrentHealthyBar/MaxHealthyBar;
}

void UHealthBarComponent::SetHealthBarPercent(float Percent)
{
	if (!IsValid(HealthBarWidget) || !IsValid(HealthBarWidget->HealthBar)) return;
	HealthBarWidget->HealthBar->SetPercent(Percent);
}

/* 受伤 */
void UHealthBarComponent::ReceiveDamage(float Damage)
{
	CurrentHealthyBar = FMath::Clamp(CurrentHealthyBar - Damage, 0.f, MaxHealthyBar);
}

void UHealthBarComponent::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	ReceiveDamage(Damage);
	float Percent = GetHealthyPercent();
	SetHealthBarPercent(Percent);
	
	if (IsAlive())
	{
		HealthBar->SetVisibility(true);
	}
	else if(!IsAlive())
	{
		HealthBar->SetVisibility(false);
	}	
	
}

bool UHealthBarComponent::IsAlive()
{
	return CurrentHealthyBar > 0.f;
}
