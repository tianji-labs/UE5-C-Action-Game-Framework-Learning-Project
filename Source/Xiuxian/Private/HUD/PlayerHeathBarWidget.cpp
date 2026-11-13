// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHeathBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Component/AttributeComponent.h"


void UPlayerHeathBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	PlayerPawn = GetOwningPlayerPawn();

	if (!IsValid(PlayerPawn)) return;
	AttributeComponent = PlayerPawn->FindComponentByClass<UAttributeComponent>();

	if (!AttributeComponent.IsValid()) return;
	AttributeComponent->OnHealthChanged.AddDynamic(this, &UPlayerHeathBarWidget::SetHealthPercent);

	SetHealthPercent(AttributeComponent->GetHealthBarPercent());
}

void UPlayerHeathBarWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (!AttributeComponent.IsValid()) return;
	AttributeComponent->OnHealthChanged.RemoveDynamic(this, &UPlayerHeathBarWidget::SetHealthPercent);
}



void UPlayerHeathBarWidget::SetHealthPercent(float Percent)
{
	if (HealthBar->GetPercent() == 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy is dead"));
		return;
	}
	HealthBar->SetPercent(Percent);
}
