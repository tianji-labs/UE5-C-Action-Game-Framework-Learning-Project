// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/AttributeComponent.h"
#include "EnemyAttributeComponent.generated.h"

/**
 * 
 */
UCLASS()
class XIUXIAN_API UEnemyAttributeComponent : public UAttributeComponent
{
	GENERATED_BODY()
private:
	virtual void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) override;
};
