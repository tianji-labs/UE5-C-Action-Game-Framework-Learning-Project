
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include"Character/StateType.h"
#include "AttributeComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XIUXIAN_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

protected:
	virtual void BeginPlay() override;

private:
	//状态
	UPROPERTY(VisibleAnywhere, Category = "Observe")
	EEnemyState EnemyState = EEnemyState::EES_NoState;
	//血条
	UPROPERTY(EditAnywhere, Category = "Attribute")
	float MaxHealth = 100.f;
	UPROPERTY(VisibleAnywhere, Category = "Observe")
	float CurrentHealth = 100.f;
	//速度
	UPROPERTY(EditAnywhere, Category = "Attribute")
	float MaxSpeed = 600.f;

public:	
/* 获取 */
	//血条
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	//AI
	FORCEINLINE float GetMaxSpeed() const { return MaxSpeed; }
	//状态
	UFUNCTION(BlueprintCallable, Category = "Attribute")
	FORCEINLINE EEnemyState GetEnemyState() const { return EnemyState; }
/* 设置 */
	//血条
	FORCEINLINE void SetMaxHealth(float NewMaxHealth) { MaxHealth = NewMaxHealth; }
	FORCEINLINE void SetCurrentHealth(float NewCurrentHealth) { CurrentHealth = NewCurrentHealth; }
	//AI
	FORCEINLINE void SetMaxSpeed(float NewMaxSpeed) { MaxSpeed = NewMaxSpeed; }
	//状态
	FORCEINLINE void ChangeBehaviorState(EEnemyState NewState) { EnemyState = NewState; }
/* 判定 */
	FORCEINLINE bool IsDead() const { return CurrentHealth <= 0.f; }


};
