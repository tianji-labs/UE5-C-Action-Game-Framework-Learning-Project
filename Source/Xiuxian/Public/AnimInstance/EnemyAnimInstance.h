// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

enum class EEnemyState : uint8;
enum class EDeathPose : uint8;

UCLASS()
class XIUXIAN_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	//初始化重写
	virtual void NativeInitializeAnimation() override;
	//更新重写
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	class AEnemy* Enemy;
	//属性组件
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	class UAttributeComponent* AttributeComponent;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	EEnemyState EnemyState;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	EDeathPose DeathPose;
	//移动组件
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* MyCharacterMovement;
	//向量
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector Velocity;
	//地面速度
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;
};
