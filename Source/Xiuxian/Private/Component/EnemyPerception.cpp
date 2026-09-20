// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyPerception.h"
#include"Enemy/Enemy.h"

void UEnemyPerception::BeginPlay()
{
	Super::BeginPlay();

	OnTargetPerceptionUpdated.AddDynamic(this, &UEnemyPerception::HandleTargetPerceptionUpdated);

	CharOwner = Cast<AEnemy>(GetOwner());
}

void UEnemyPerception::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnTargetPerceptionUpdated.RemoveAll(this);
}

void UEnemyPerception::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		CharOwner->StartChaseTarget(Actor);
	}
	else if (!Stimulus.WasSuccessfullySensed())
	{
		CharOwner->EndChaseTarget();
	}
}

void UEnemyPerception::ClearPerception()
{
	OnTargetPerceptionUpdated.RemoveAll(this);
}
