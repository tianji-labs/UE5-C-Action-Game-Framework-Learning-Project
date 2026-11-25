
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidgetComponent.generated.h"

class UHealthBarWidget;
class UEnemyAttributeComponent;

UCLASS()
class XIUXIAN_API UHealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "HealthBar")
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass; //À¶Í¼
	UPROPERTY()
	TObjectPtr<UHealthBarWidget> HealthBarWidget; //ÊµÀý

	TWeakObjectPtr<UEnemyAttributeComponent> EnemyAttributeComponent;

	UFUNCTION()
	void VisibilityHandle(float NewHealthPercent);


};
