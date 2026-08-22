// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class UButton;

UCLASS()
class XIUXIAN_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* Start;
	UPROPERTY(meta = (BindWidget))
	UButton* Setting;
	UPROPERTY(meta = (BindWidget))
	UButton* End;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void EndGame();

};
