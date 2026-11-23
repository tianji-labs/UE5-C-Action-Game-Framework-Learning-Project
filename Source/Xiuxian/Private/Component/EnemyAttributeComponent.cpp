// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyAttributeComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Enemy/Enemy.h"

void UEnemyAttributeComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Super::HandleTakeAnyDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);

	AEnemy* Enemy = Cast<AEnemy>(GetOwner());
	if (!IsValid(Enemy) || !IsValid(Enemy->GetBlackboardComponent())) return;
	if (!IsDead()) return;
	//ËÀÍöºóÉèÖÃºÚ°åÖµIsDead
	Enemy->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);
}
