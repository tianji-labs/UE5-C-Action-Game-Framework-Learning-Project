// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Component/EnemyAttributeComponent.h"
#include "Enemy/Enemy.h"


void UHealthBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UHealthBarWidget::InitializeEnemy(AEnemy* Enemy)
{	
	//获取EnemyAttributeComponent
	if (!IsValid(Enemy)) return;
	EnemyAttributeComponent = Enemy->FindComponentByClass<UEnemyAttributeComponent>();

	//接收EnemyAttributeComponent的广播
	if (!IsValid(EnemyAttributeComponent))  return;
	EnemyAttributeComponent->OnEnemyHealthChanged.AddUniqueDynamic(this, &UHealthBarWidget::HealthChange);
	HealthBar->SetPercent(EnemyAttributeComponent->GetHealthBarPercent());
}

void UHealthBarWidget::NativeDestruct()
{
	//移除委托
	EnemyAttributeComponent->OnEnemyHealthChanged.RemoveAll(this);

	Super::NativeDestruct();
}

void UHealthBarWidget::HealthChange(float NewHealthPercent)
{
	if (!IsValid(HealthBar)) return;
	HealthBar->SetPercent(NewHealthPercent);
}


