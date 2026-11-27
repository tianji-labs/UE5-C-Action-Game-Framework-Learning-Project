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
#include "Kismet/GameplayStatics.h"
//自
#include "HUD/StartMenu.h"
#include "Item/Weapon.h"
#include "Component/Combat.h"
#include "Component/AttributeComponent.h"


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

	//combat组件
	Combat = CreateDefaultSubobject<UCombat>(TEXT("Combat"));
	//属性组件
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

}

void ATianji::BeginPlay()
{
	Super::BeginPlay();

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
	EnhancedInput->BindAction(IA_Menu, ETriggerEvent::Started, this, &ATianji::Menu);
	EnhancedInput->BindAction(IA_Pickup, ETriggerEvent::Started, this, &ATianji::Pickup);
	EnhancedInput->BindAction(IA_Attack, ETriggerEvent::Started, this, &ATianji::Attack);


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

	if (!Controller || Combat->GetPlayerCombatState()!=EPlayerCombatState::EPCS_Unoccupied || Combat->GetPlayerLifeState() != EPlayerLifeState::EPLS_Alive) return;

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
	if (Combat->GetPlayerCombatState() != EPlayerCombatState::EPCS_Unoccupied || Combat->GetPlayerLifeState() != EPlayerLifeState::EPLS_Alive) return;
	Super::Jump();
}

void ATianji::Menu()
{
	//广播
	OnOpenMenuRequest.Broadcast();
}

/* 拾取 */
void ATianji::Pickup()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

	PickupWeapon(OverlappingWeapon);
}

void ATianji::PickupWeapon(AWeapon* Weapon)
{
	if (Weapon)
	{
		Weapon->Equip(GetMesh(), FName("R_HandSocket"), this, this);
		OverlappingItem = nullptr;
		Weapon->SetSphereNocollision();
		Weapon->SetWeaponCollisionEnable(false);
		WeaponState = EWeaponState::EWS_CloseCombatOneHand;
		EquippedWeapon = Weapon;
	}
}

/* 攻击 */
void ATianji::Attack()
{
	if (!IsValid(Combat)) return;
	Combat->HandleAttack();
}
	
/* 受击 */
void ATianji::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (!IsValid(Combat)) return;
	//处理死亡
	if (IsValid(AttributeComponent) && AttributeComponent->IsDead())
	{
		Combat->HandleDeath(Hitter);
	}
	//处理受击
	Combat->HandleGetHit(Hitter);

	if (!IsValid(GetHitSound)) return;
	UGameplayStatics::PlaySoundAtLocation(this, GetHitSound, ImpactPoint);
}



