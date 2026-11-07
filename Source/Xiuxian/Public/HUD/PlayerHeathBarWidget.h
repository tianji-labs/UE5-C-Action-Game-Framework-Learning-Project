// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHeathBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class XIUXIAN_API UPlayerHeathBarWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UTextBlock> HealthText;

public:
	void SetHealthPercent(float Percent);

};
