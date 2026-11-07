// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHeathBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UPlayerHeathBarWidget::SetHealthPercent(float Percent)
{
	HealthBar->SetPercent(Percent);
}
