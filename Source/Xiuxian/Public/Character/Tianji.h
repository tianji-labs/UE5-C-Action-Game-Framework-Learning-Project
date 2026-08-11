// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tianji.generated.h"

//移动
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
//
class UCharacterMovementComponent;

//第三人称视角
class USpringArmComponent;
class UCameraComponent;

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
/*第三人称视角*/
	void Look(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, Category = "PersonalAsset|Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "PersonalAsset|Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Camera")
	UInputAction* IA_Look;

/*移动*/
	void Forward(const FInputActionValue& Value);
	void Right(const FInputActionValue& Value);
	void Jump();

	//UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Movement")
	UInputMappingContext* IMC_Tianji;
	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Movement")
	UInputAction* IA_Forward;
	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Movement")
	UInputAction* IA_Right;
	UPROPERTY(EditDefaultsOnly,Category = "PersonalAsset|Movement")
	UInputAction* IA_Jump;

};
