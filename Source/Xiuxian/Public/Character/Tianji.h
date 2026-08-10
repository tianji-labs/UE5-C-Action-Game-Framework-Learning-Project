// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tianji.generated.h"

//ÒÆ¶¯
class UCharacterMovementComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


UCLASS()
class XIUXIAN_API ATianji : public ACharacter
{
	GENERATED_BODY()

public:
	ATianji();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;


private:
/*ÒÆ¶¯*/
	void Forward(const FInputActionValue& Value);
	void Right(const FInputActionValue& Value);

	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Movement")
	UInputMappingContext* IMC_Tianji;
	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Movement")
	UInputAction* IA_Forward;
	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Movement")
	UInputAction* IA_Right;

};
