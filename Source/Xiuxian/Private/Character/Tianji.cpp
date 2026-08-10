// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Tianji.h"
//移动
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/CharacterMovementComponent.h"

ATianji::ATianji()
{
	MovementComponent = GetCharacterMovement();

	PrimaryActorTick.bCanEverTick = true;

}

void ATianji::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Tianji, 0);
		}
	}
	
}

void ATianji::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATianji::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//增强输入
	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	//绑定回调
	EnhancedInput->BindAction(IA_Forward, ETriggerEvent::Triggered, this, &ATianji::Forward);
	EnhancedInput->BindAction(IA_Right, ETriggerEvent::Triggered, this, &ATianji::Right);

}

//前后
void ATianji::Forward(const FInputActionValue& Value)
{
	float Val = Value.Get<float>();
	if (Controller && Val != 0)
	{
		const FRotator YawRot(0, Controller->GetControlRotation().Yaw, 0);
		const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);

		AddMovementInput(ForwardDir, Val);
	}
}
//左右
void ATianji::Right(const FInputActionValue& Value)
{
	float Val = Value.Get<float>();
	if (Controller && Val != 0)
	{
		const FRotator YawRot(0, Controller->GetControlRotation().Yaw, 0);
		const FVector RightDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

		AddMovementInput(RightDir, Val);
	}
}

