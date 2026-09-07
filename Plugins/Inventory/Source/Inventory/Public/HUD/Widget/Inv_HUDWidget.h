// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/* 拾取信息 */
	//表现类在蓝图实现
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void ShowPickupMessage(const FString& Message);
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void HidePickupMessage();
};
