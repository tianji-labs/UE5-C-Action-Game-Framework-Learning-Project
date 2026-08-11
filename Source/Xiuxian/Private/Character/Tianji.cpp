// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Tianji.h"
//移动
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//#include "GameFramework/FloatingPawnMovement.h"
//#include "GameFramework/CharacterMovementComponent.h"

//第三人称视角
#include"Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


ATianji::ATianji()
{
	//MovementComponent = GetCharacterMovement();
	PrimaryActorTick.bCanEverTick = true;
	//第三人称视角
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;  //弹簧臂跟随玩家视角
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

void ATianji::BeginPlay()
{
	Super::BeginPlay();
	//添加映射上下文
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
	EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ATianji::Jump);
	EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ATianji::Look);


}
//视角
void ATianji::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();

	if (Controller)
	{
		// 水平旋转（Yaw）：改变 Pawn 的朝向
		AddControllerYawInput(LookInput.X);
		// 垂直旋转（Pitch）：改变相机俯仰
		AddControllerPitchInput(-LookInput.Y);
	}
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
//跳跃
void ATianji::Jump()
{
	Super::Jump();
}

