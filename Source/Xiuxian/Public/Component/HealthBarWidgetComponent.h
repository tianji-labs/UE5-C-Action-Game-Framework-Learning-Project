
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidgetComponent.generated.h"

class UHealthBarWidget;

UCLASS()
class XIUXIAN_API UHealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "HealthBar")
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass; //À¶Í¼
	UPROPERTY()
	TObjectPtr<UHealthBarWidget> HealthBarWidget; //ÊµÀý


};
