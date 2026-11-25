// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/HealthBarWidgetComponent.h"
#include "HUD/HealthBarWidget.h"
#include "Enemy/Enemy.h"
#include "Component/EnemyAttributeComponent.h"

void UHealthBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	//在敌人头顶显示血条
	if (!IsValid(HealthBarWidgetClass)) return;
	HealthBarWidget = CreateWidget<UHealthBarWidget>(GetWorld(), HealthBarWidgetClass);  //根据指定的 Widget 类创建实际的 Widget 对象

	if ( !IsValid(HealthBarWidget)) return;
	SetWidget(HealthBarWidget); 
	SetWidgetSpace(EWidgetSpace::Screen);  
	HealthBarWidget->SetVisibility(ESlateVisibility::Hidden);

	//初始化Widget
	AEnemy* Enemy = Cast<AEnemy>(GetOwner());
	HealthBarWidget->InitializeEnemy(Enemy);

	//绑定血条可见性函数
	if (!IsValid(Enemy)) return;
	EnemyAttributeComponent = Enemy->FindComponentByClass<UEnemyAttributeComponent>();

	if (!EnemyAttributeComponent.IsValid()) return;
	EnemyAttributeComponent->OnEnemyHealthChanged.AddUniqueDynamic(this, &UHealthBarWidgetComponent::VisibilityHandle);
}

void UHealthBarWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EnemyAttributeComponent.IsValid())
	{
		EnemyAttributeComponent->OnEnemyHealthChanged.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void UHealthBarWidgetComponent::VisibilityHandle(float NewHealthPercent)
{
	UE_LOG(LogTemp, Warning, TEXT("1111"));
	if (NewHealthPercent > 0.f)
	{
		HealthBarWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
