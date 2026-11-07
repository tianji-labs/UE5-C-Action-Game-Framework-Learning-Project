// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UPlayerHeathBarWidget;
class UInputMappingContext;
class UInventoryInteractionComponent;

UCLASS()
class XIUXIAN_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	/* 组件 */
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UInventoryInteractionComponent> InventoryInteractionComponent;
	/* 输入 */ 
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Tianji;
	/* 血条 */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UPlayerHeathBarWidget> PlayerHealthBarWidgetClass;
	UPROPERTY()
	TObjectPtr<UPlayerHeathBarWidget> PlayerHealthBarWidget;
};
