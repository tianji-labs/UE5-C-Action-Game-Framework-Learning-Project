// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/Task/BTTask_AttackHandle.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Component/EnemyAIController.h"
#include "Enemy/Enemy.h"

UBTTask_AttackHandle::UBTTask_AttackHandle()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_AttackHandle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//获取行为树组件
	BehaviorTreeComp = &OwnerComp;

	//获取Enemy
	if (!IsValid(OwnerComp.GetAIOwner())) return EBTNodeResult::Failed;
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	FOnMontageEnded OnAttackEnded;
	OnAttackEnded.BindUObject(this, &UBTTask_AttackHandle::OnMontageEnded);

	if (!Enemy->PlayAttackMontage(OnAttackEnded))
	{
		BehaviorTreeComp = nullptr;
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;

}

void UBTTask_AttackHandle::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{	
	if (!IsValid(BehaviorTreeComp)) return;

	FinishLatentTask(*BehaviorTreeComp,bInterrupted? EBTNodeResult::Failed: EBTNodeResult::Succeeded);

	BehaviorTreeComp = nullptr;
}