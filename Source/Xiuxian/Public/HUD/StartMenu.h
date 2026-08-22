// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StartMenu.generated.h"

class UButton;

UCLASS()
class XIUXIAN_API AStartMenu : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "PersonalAsset")
	TSubclassOf<class UMenuWidget> MenuWidgetClass;
};
