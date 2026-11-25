// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Task/BTTask_GetHitHandle.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Component/EnemyAIController.h"
#include "Enemy/Enemy.h"

EBTNodeResult::Type UBTTask_GetHitHandle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BehaviorTreeComp = &OwnerComp;

	if (!IsValid(OwnerComp.GetAIOwner())) return EBTNodeResult::Failed;
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	//获取TargetActor
	if(!IsValid(Enemy) || !IsValid(Blackboard)) return EBTNodeResult::Failed;
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

	//绑定回调函数
	FOnMontageEnded OnGetHitEnded;
	OnGetHitEnded.BindUObject(this, &UBTTask_GetHitHandle::OnGetHitMontageEnded);

	//播放受击动画
	if (!Enemy->PlayGetHitMontage(TargetActor, OnGetHitEnded))
	{
		BehaviorTreeComp = nullptr;
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_GetHitHandle::OnGetHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsValid(BehaviorTreeComp)) return;

	FinishLatentTask(*BehaviorTreeComp, bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);

	BehaviorTreeComp->GetBlackboardComponent()->SetValueAsBool(FName("IsGetHit"), false);

	BehaviorTreeComp = nullptr;
}
