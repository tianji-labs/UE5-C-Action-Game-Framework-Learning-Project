// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHeathBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Component/AttributeComponent.h"


void UPlayerHeathBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (!IsValid(PlayerPawn)) return;
	UAttributeComponent* AttributeComponent = PlayerPawn->FindComponentByClass<UAttributeComponent>();
	if (!IsValid(AttributeComponent)) return;
	AttributeComponent->OnHealthChanged.AddDynamic(this, &UPlayerHeathBarWidget::SetHealthPercent);

	SetHealthPercent(AttributeComponent->GetHealthBarPercent());
}

void UPlayerHeathBarWidget::SetHealthPercent(float Percent)
{
	HealthBar->SetPercent(Percent);
}
