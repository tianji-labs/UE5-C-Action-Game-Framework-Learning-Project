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
//自
#include "HUD/StartMenu.h"
#include "Item/Weapon.h"
#include "Component/Combat.h"


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

}

void ATianji::SetActionState(EActionState NewState)
{
	ActionState = NewState;
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
	EnhancedInput->BindAction(IA_Attack, ETriggerEvent::Started, this, &ATianji::CharacterAttack);


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

	if (!Controller || ActionState != EActionState::EAS_Unoccupied) return;

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
	if (ActionState != EActionState::EAS_Unoccupied) return;
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
		WeaponState = EWeaponState::EWS_CloseCombatOneHand;
		OverlappingItem = nullptr;
		Weapon->SetSphereNocollision();
		Weapon->SetWeaponCollisionEnable(false);
		EquipWeapon = Weapon;
	}
}

/* 攻击 */
void ATianji::CharacterAttack()
{
	if (ActionState == EActionState::EAS_Attacking || WeaponState == EWeaponState::EWS_Unequipped) return;
	PlayMontageSection(AttackMontage, AttackMontageSections[0]);
	ActionState = EActionState::EAS_Attacking;
}

/* 受击 */
void ATianji::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	GetHitAnim(Hitter);
}

void ATianji::GetHitAnim(AActor* Hitter)
{

	const double Theta = ObtainAngleForAnim(Hitter);

	FName Section = FName("FromBack");

	if (Theta < 45.f && Theta > -45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta < -135.f && Theta > 135.f)
	{
		Section = FName("FromBack");
	}
	else if (Theta <= -45.f && Theta >= -135.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta <= 135.f && Theta >= 45.f)
	{
		Section = FName("FromRight");
	}

	if (!IsValid(GetHitMontage)) return;
	PlaySectionMontage(Section, GetHitMontage);
}

double ATianji::ObtainAngleForAnim(AActor* Hitter)
{
	//获取自身单位向量
	const FVector Forward = GetActorForwardVector();
	//使向量处于同一平面
	const FVector HitterLocation = Hitter->GetActorLocation();
	const FVector PointLowerd = FVector(HitterLocation.X, HitterLocation.Y, GetActorLocation().Z);
	//获得自身到Hitter的单位向量
	const FVector ToHit = (PointLowerd - GetActorLocation()).GetSafeNormal();

	//单位向量点积获得cos
	double CosTheta = FVector::DotProduct(Forward, ToHit);
	//cos转弧度
	CosTheta = FMath::Clamp(CosTheta, -1.0, 1.0);
	double Theta = FMath::Acos(CosTheta);
	//弧度转角度
	Theta = FMath::RadiansToDegrees(Theta);

	//右手坐标系调整方向
	const FVector CrossProduct = FVector::CrossProduct(HitterLocation, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	return Theta;
}

//播放某段蒙太奇动画
void ATianji::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage); 
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

AWeapon* ATianji::GetEquippedWeapon() const
{
	return EquipWeapon;
}


/* 通用 */
void ATianji::PlaySectionMontage(FName Section, UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance) || !IsValid(Montage)) return;
	AnimInstance->Montage_Play(Montage);
	AnimInstance->Montage_JumpToSection(Section, Montage);
}