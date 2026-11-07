// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Inv_PlayController.h"
#include "MainPlayerController.generated.h"

class UPlayerHeathBarWidget;

UCLASS()
class XIUXIAN_API AMainPlayerController : public AInv_PlayController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	/* ÑªÌõ */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UPlayerHeathBarWidget> PlayerHealthBarWidgetClass;
	UPROPERTY()
	TObjectPtr<UPlayerHeathBarWidget> PlayerHealthBarWidget;
};
