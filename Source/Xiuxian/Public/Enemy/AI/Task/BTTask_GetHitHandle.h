// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetHitHandle.generated.h"

/**
 * 
 */
UCLASS()
class XIUXIAN_API UBTTask_GetHitHandle : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UBehaviorTreeComponent* BehaviorTreeComp = nullptr;

	void OnGetHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
