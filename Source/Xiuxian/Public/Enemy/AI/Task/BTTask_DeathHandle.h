// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DeathHandle.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class XIUXIAN_API UBTTask_DeathHandle : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	FOnMontageEnded OnDeathMontageEnded;
	UBehaviorTreeComponent* BehaviorTreeComp = nullptr;

	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);
};
