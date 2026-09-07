// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//½Ó¿Ú
#include "Inv_Highlightable.h"
#include "Components/StaticMeshComponent.h"
#include "Inv_HighlightableStaticMesh.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_HighlightableStaticMesh : public UStaticMeshComponent,public IInv_Highlightable
{
	GENERATED_BODY()
	
public:
	virtual void Highlight_Implementation() override;
	virtual void UnHighlight_Implementation() override;

private:
	//²ÄÖÊ
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UMaterialInterface> HighlightMaterial;

};
