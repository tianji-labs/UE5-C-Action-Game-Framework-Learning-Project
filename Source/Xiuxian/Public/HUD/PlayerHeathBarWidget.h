// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHeathBarWidget.generated.h"

class APawn;
class UProgressBar;
class UTextBlock;
class UAttributeComponent;

UCLASS()
class XIUXIAN_API UPlayerHeathBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	TObjectPtr<APawn> PlayerPawn;
	TWeakObjectPtr<UAttributeComponent> AttributeComponent;

/*ÑªÌõ*/ 
	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UTextBlock> HealthText;

public:
	UFUNCTION()
	void SetHealthPercent(float Percent);

};
