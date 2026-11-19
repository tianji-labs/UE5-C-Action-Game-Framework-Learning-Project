// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthBarWidget;
class UHealthBar;
class UAttributeComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XIUXIAN_API UHealthBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
/* 构造 */
	UHealthBarComponent();

protected:
/* Begin */
	virtual void BeginPlay() override;

private:
/* 组件 */
	UPROPERTY()
	TObjectPtr<AActor> Owner;
	UPROPERTY(VisibleAnywhere, Category = "HealthBar")
	TObjectPtr<UHealthBar> HealthBar;
	UPROPERTY(VisibleAnywhere, Category = "HealthBar")
	TWeakObjectPtr<UAttributeComponent> AttributeComponent;

/* 血条 */
	UPROPERTY(EditDefaultsOnly, Category = "HealthBar")
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass; //蓝图
	UPROPERTY()
	TObjectPtr<UHealthBarWidget> HealthBarWidget; //实例

	void CreateHealthBarWidget();

	UFUNCTION()
	void HandleHealthBar(float NewHealthPercent);
	void SetHealthBarPercent(float Percent);




};
