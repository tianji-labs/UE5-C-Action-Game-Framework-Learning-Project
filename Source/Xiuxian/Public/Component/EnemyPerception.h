// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "EnemyPerception.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FPerceptionUpdated, AActor*, bool);

class AEnemy;

UCLASS()
class XIUXIAN_API UEnemyPerception : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:


protected:

	virtual void BeginPlay() override;

private:
	TObjectPtr<AEnemy> CharOwner;

	UFUNCTION()
	void HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);



public:
	//Çå³ý
	void ClearPerception();

	//perceptionÎ¯ÍÐÊµÀý
	FPerceptionUpdated PerceptionUpdated;
};
