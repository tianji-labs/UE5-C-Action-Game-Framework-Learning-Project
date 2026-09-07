// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Component/Inv_InventoryComponent.h"

#include"HUD/Widget/Iventory/IventoryBase/Inv_IventoryBase.h"



UInv_InventoryComponent::UInv_InventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}


void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructIventory();
	
}

void UInv_InventoryComponent::ConstructIventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Iventory Component should have a Player as Owner."));
	if (!OwningController->IsLocalController()) return;

	InventoryMenu = CreateWidget<UInv_IventoryBase>(OwningController.Get(), InventoryMenuClass);
	InventoryMenu->AddToViewport();


}



