// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UpdateSpeed.generated.h"

/**
 * 
 */
UCLASS()
class XIUXIAN_API UUpdateSpeed : public UBTService
{
	GENERATED_BODY()
	
public:
	UUpdateSpeed();

protected:

	//激活时调用
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float NewSpeed;
};
