// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthBarWidget;
class UHealthBar;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XIUXIAN_API UHealthBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
/* 构造 */
	UHealthBarComponent();
/* Tick */
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
/* Begin */
	virtual void BeginPlay() override;
/* EndPlay */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
/* 组件 */
	UPROPERTY()
	TObjectPtr<AActor> Owner;
	UPROPERTY(VisibleAnywhere, Category = "HealthBar")
	TObjectPtr<UHealthBar> HealthBar;

/* 血条UI */
	UPROPERTY(EditDefaultsOnly, Category = "HealthBar")
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass; //蓝图
	UPROPERTY()
	TObjectPtr<UHealthBarWidget> HealthBarWidget; //实例

	void CreateHealthBarWidget();

/*血条属性*/
	UPROPERTY(EditAnywhere, Category = "HealthBar")
	float MaxHealthyBar = 100.f;
	UPROPERTY(VisibleAnywhere, Category = "HealthBar")
	float CurrentHealthyBar = 100.f;

/* 血条百分比 */

	float GetHealthyPercent();
	void SetHealthBarPercent(float Percent);

/* 受伤 */
	void ReceiveDamage(float Damage);
	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:
	bool IsAlive();




};
