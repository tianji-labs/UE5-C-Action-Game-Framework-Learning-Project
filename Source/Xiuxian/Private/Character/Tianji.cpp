// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Tianji.h"
//移动
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//#include "GameFramework/FloatingPawnMovement.h"
//人物旋转
#include "GameFramework/CharacterMovementComponent.h"
//第三人称视角
#include"Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
//数学库（计算）
#include "Kismet/KismetMathLibrary.h"


ATianji::ATianji()
{

	PrimaryActorTick.bCanEverTick = true;
	//第三人称视角
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;  //弹簧臂跟随玩家视角

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//人物旋转
	MovementComponent = GetCharacterMovement();
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->RotationRate = FRotator(0.f, 360.f, 0.f);
	bUseControllerRotationYaw = false;  //本体不随控制器转动

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
	EnhancedInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ATianji::Move);
	EnhancedInput->BindAction(IA_Move, ETriggerEvent::Completed, this, &ATianji::Release);
	EnhancedInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &ATianji::Jump);
	EnhancedInput->BindAction(IA_Sprint, ETriggerEvent::Started, this, &ATianji::Sprint);
	EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ATianji::Look);


}

/*第三人称视角*/
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



/*移动*/
void ATianji::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (!Controller) return;

	const FRotator YawRot(0, Controller->GetControlRotation().Yaw, 0);
	const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	const FVector RightDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
	AddMovementInput(ForwardDir, MovementVector.X);
	AddMovementInput(RightDir, MovementVector.Y);

}

void ATianji::Release(const FInputActionValue& Value)
{
	MovementComponent = GetCharacterMovement();
	if (MovementComponent && UKismetMathLibrary::VSizeXY(MovementComponent->Velocity) > 0.f)
	{
		MovementComponent->MaxWalkSpeed = 400.f;
	}
}
//冲刺
void ATianji::Sprint()
{
	MovementComponent = GetCharacterMovement();
	if (MovementComponent && UKismetMathLibrary::VSizeXY(MovementComponent->Velocity) > 0.f)
	{
		MovementComponent->MaxWalkSpeed = 600.f;
	}
}

void ATianji::Jump()
{
	Super::Jump();
}

