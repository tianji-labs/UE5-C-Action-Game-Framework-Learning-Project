// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyPerception.h"

void UEnemyPerception::BeginPlay()
{
	Super::BeginPlay();

	OnTargetPerceptionUpdated.AddDynamic(this, &UEnemyPerception::HandleTargetPerceptionUpdated);

}


void UEnemyPerception::HandleTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	bool bSensed = Stimulus.WasSuccessfullySensed();
	
	PerceptionUpdated.Broadcast(Actor, bSensed);
	
}

void UEnemyPerception::ClearPerception()
{
	OnTargetPerceptionUpdated.RemoveAll(this);
}
