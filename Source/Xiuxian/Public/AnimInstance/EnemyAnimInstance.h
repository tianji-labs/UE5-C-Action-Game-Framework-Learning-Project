// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

enum class EDeathPose : uint8;

class AEnemy;
class UCharacterMovementComponent;

UCLASS()
class XIUXIAN_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	TWeakObjectPtr<AEnemy> Enemy;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TWeakObjectPtr<UCharacterMovementComponent> MyCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	EDeathPose DeathPose;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;
};
