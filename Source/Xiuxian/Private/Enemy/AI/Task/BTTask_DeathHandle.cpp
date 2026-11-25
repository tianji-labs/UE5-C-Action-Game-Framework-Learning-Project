// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Task/BTTask_DeathHandle.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Component/EnemyAIController.h"
#include "Enemy/Enemy.h"

EBTNodeResult::Type UBTTask_DeathHandle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//获取Enemy与blackboard
	if (!IsValid(OwnerComp.GetAIOwner())) return EBTNodeResult::Failed;
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(Enemy) || !IsValid(Blackboard)) return EBTNodeResult::Failed;
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));

	//绑定回调
	OnDeathMontageEnded.BindUObject(this, &UBTTask_DeathHandle::OnMontageEnd);

	//播放死亡动画，若失败返回failed
	if (!Enemy->PlayDeathMontage(TargetActor, OnDeathMontageEnded))
	{
		BehaviorTreeComp = nullptr;
		return EBTNodeResult::Failed;
	}
	//死亡处理
	Enemy->Die();

	return EBTNodeResult::InProgress;
}

void UBTTask_DeathHandle::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (!IsValid(BehaviorTreeComp)) return;

	FinishLatentTask(*BehaviorTreeComp, bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);

	BehaviorTreeComp = nullptr;
}
