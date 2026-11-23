// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyPerception.h"
#include"BehaviorTree/BlackboardComponent.h"
#include "Enemy/Enemy.h"

void UEnemyPerception::BeginPlay()
{
	Super::BeginPlay();

	OnTargetPerceptionUpdated.AddDynamic(this, &UEnemyPerception::HandleTargetPerceptionUpdated);

	Enemy = Cast<AEnemy>(GetOwner());
}



void UEnemyPerception::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	bool bSensed = Stimulus.WasSuccessfullySensed();
	
	if (Actor->ActorHasTag(FName("Player")) && bSensed)
	{
		//目标被感知，取消延迟后追击
		GetWorld()->GetTimerManager().ClearTimer(CancelChaseHandle);
		//设置追击相关黑板值
		Enemy->GetBlackboardComponent()->SetValueAsObject(FName("ActorTarget"), Actor);
	}
	else
	{
		//目标丢失，延迟2秒后取消追击
		GetWorld()->GetTimerManager().SetTimer(CancelChaseHandle, this, &UEnemyPerception::CancelChase, 2.f);
	}
	
}

void UEnemyPerception::CancelChase()
{
	//设置取消追击相关黑板值
	Enemy->GetBlackboardComponent()->SetValueAsObject(FName("ActorTarget"), nullptr);
}
