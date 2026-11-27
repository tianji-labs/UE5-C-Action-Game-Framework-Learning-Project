// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/HealthBarWidgetComponent.h"
#include "HUD/HealthBarWidget.h"
#include "Enemy/Enemy.h"
#include "Component/EnemyAttributeComponent.h"
#include "Component/EnemyPerception.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	//获取组件
	if (!IsValid(Enemy)) return;
	EnemyAttributeComponent = Enemy->FindComponentByClass<UEnemyAttributeComponent>();
	EnemyPerception = Enemy->FindComponentByClass<UEnemyPerception>();
	UBlackboardComponent* Blackboard = Enemy->GetBlackboardComponent();
	
	//获取TargetActor
	ActorTarget = Cast<AActor>(Blackboard->GetValueAsObject(FName("ActorTarget")));

	//绑定可见性组件
	if (!EnemyAttributeComponent.IsValid()) return;
	EnemyAttributeComponent->OnEnemyHealthChanged.AddUniqueDynamic(this, &UHealthBarWidgetComponent::VisibilityHandle);

	//绑定目标丢失函数——隐藏血条
	if (!EnemyPerception.IsValid()) return;
	EnemyPerception->OnTrackLoss.AddUObject(this, &UHealthBarWidgetComponent::TrackLossHandle);
}

void UHealthBarWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EnemyAttributeComponent.IsValid())
	{
		EnemyAttributeComponent->OnEnemyHealthChanged.RemoveAll(this);
	}

	if (EnemyPerception.IsValid())
	{
		EnemyPerception->OnTrackLoss.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void UHealthBarWidgetComponent::VisibilityHandle(float NewHealthPercent)
{
	if (NewHealthPercent > 0.f)
	{
		HealthBarWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		HealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UHealthBarWidgetComponent::TrackLossHandle()
{
	if (!IsValid(ActorTarget))
	{
		HealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
