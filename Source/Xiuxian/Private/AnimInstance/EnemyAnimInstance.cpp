// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/EnemyAnimInstance.h"
#include"Enemy/Enemy.h"
#include"Component/AttributeComponent.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Enemy = Cast<AEnemy>(TryGetPawnOwner());
	if(IsValid(Enemy))
	{
		AttributeComponent = Enemy->FindComponentByClass<UAttributeComponent>();
		MyCharacterMovement = Enemy->FindComponentByClass<UCharacterMovementComponent>();
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (MyCharacterMovement)
	{
		Velocity = MyCharacterMovement->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
		DeathPose = Enemy->GetDeathPose();
	}
}
