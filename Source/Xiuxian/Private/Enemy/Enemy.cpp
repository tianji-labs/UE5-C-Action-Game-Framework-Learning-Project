// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include"Components/CapsuleComponent.h"
#include "Component/EnemyPerception.h"
#include"Perception/AISenseConfig_Sight.h"
#include"Component/EnemyAttributeComponent.h"
#include"Component/EnemyAIController.h"
#include "Component/HealthBarWidgetComponent.h"
#include"Item/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include"AnimInstance/EnemyAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	//感知
	EnemyPerception = CreateDefaultSubobject<UEnemyPerception>(TEXT("EnemyPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.f; //视距半径（能看多远）
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->PeripheralVisionAngleDegrees = 45.f;//能看到多大角度
	SightConfig->DetectionByAffiliation.bDetectEnemies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //检测中立
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;//检测友方
	EnemyPerception->ConfigureSense(*SightConfig);//配置感知（sense）
	EnemyPerception->SetDominantSense(SightConfig->GetSenseImplementation());
	/*
	GetSenseImplementation 获得配置对应的 Sense 类型  AISense_Sight
	SetDominantSense 把 Sense 设置为主要/主导感知类型
	*/
	//属性
	EnemyAttributeComponent = CreateDefaultSubobject<UEnemyAttributeComponent>(TEXT("EnemyAttributeComp"));
	//攻击范围
	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeSphere"));
	AttackRangeSphere->SetupAttachment(GetRootComponent());
	AttackRangeSphere->InitSphereRadius(AttackRadius);
	//UI
	HealthBarWidgetComponent = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBarWidgetComponent"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Enemy"));

	EquipDefaultWeapon();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance)) return;
	AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &AEnemy::SetWeaponCollidion);

	if (!IsValid(AttackRangeSphere)) return;
	AttackRangeSphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &AEnemy::AttackRangeBeginOverlap);
	AttackRangeSphere->OnComponentEndOverlap.AddUniqueDynamic(this, &AEnemy::AttackRangeEndOverlap);

}

//攻击范围检测
void AEnemy::AttackRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	if (!GetBlackboardComponent()) return;
	GetBlackboardComponent()->SetValueAsBool(FName("InAttackRange"), true);
}

void AEnemy::AttackRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!GetBlackboardComponent()) return;
	GetBlackboardComponent()->SetValueAsBool(FName("InAttackRange"), false);
}

//修改速度
void AEnemy::UpdateSpeed(float NewSpeed)
{
	if (!IsValid(GetCharacterMovement())) return;
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

//受击相关
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{

	//受击时修改黑板值IsGethit
	if (!IsValid(GetBlackboardComponent())) return;
	GetBlackboardComponent()->SetValueAsObject(FName("ActorTarget"), Hitter);
	GetBlackboardComponent()->SetValueAsBool(FName("IsGetHit"), true);
	//播放受击音效
	if (!IsValid(GetHitSound)) return;
	UGameplayStatics::PlaySoundAtLocation(this, GetHitSound, ImpactPoint);
}

//获取黑板
UBlackboardComponent* AEnemy::GetBlackboardComponent() const
{
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());

	if (!IsValid(EnemyAIController)) return nullptr;
	return EnemyAIController->GetBlackboardComponent();
}

void AEnemy::Die()
{

	SetLifeSpan(5.f);

	if (!IsValid(GetCharacterMovement())) return;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = false;


	if (IsValid(GetCapsuleComponent()))
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (!IsValid(DeafaultWeapon)) return;
	DeafaultWeapon->SetBoxNoCollision();
	DeafaultWeapon->SetLifeSpan(5.f);
}

//播放动画并设置结束委托
bool AEnemy::PlayAttackMontage(const FOnMontageEnded& EndDelegate)
{
	if (!IsValid(AttackMontage)) return false;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance)) return false;

	if (AnimInstance->Montage_Play(AttackMontage) <= 0.f) return false;

	FOnMontageEnded Delegate = EndDelegate;
	AnimInstance->Montage_SetEndDelegate(Delegate, AttackMontage);

	return true;

}

bool AEnemy::PlayGetHitMontage(AActor* Hitter, const FOnMontageEnded& EndDelegate)
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

	//播放失败返回false
	if(!PlaySectionMontage(Section, GetHitMontage)) return false;

	//设置动画结束委托
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance)) return false;

	FOnMontageEnded Delegate = EndDelegate;
	AnimInstance->Montage_SetEndDelegate(Delegate, GetHitMontage);

	return true;

}

bool AEnemy::PlayDeathMontage(AActor* Hitter, const FOnMontageEnded& EndDelegate)
{
	const double Theta = ObtainAngleForAnim(Hitter);

	FName Section = FName("ForBack");

	if (Theta <= -90.f || Theta >= 90.f)
	{
		Section = FName("ForFront");
		DeathPose = EDeathPose::EDP_DeathFront;
	}
	else
	{
		Section = FName("ForBack");
		DeathPose = EDeathPose::EDP_DeathBack;
	}
	//播放失败返回false
	if (!PlaySectionMontage(Section, DeadMontage)) return false;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance)) return false;

	FOnMontageEnded Delegate = EndDelegate;
	AnimInstance->Montage_SetEndDelegate(Delegate, DeadMontage);

	return true;
}

double AEnemy::ObtainAngleForAnim(AActor* Hitter)
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

/* 武器 */
void AEnemy::EquipDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && IsValid(WeaponClass))
	{
		DeafaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DeafaultWeapon->Equip(GetMesh(), FName("RightHandWeapon"), this, this);
		DeafaultWeapon->SetSphereNocollision();
	}
}

void AEnemy::SetWeaponCollidion(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (!IsValid(DeafaultWeapon)) return;

	if (NotifyName == FName("EnableCollision"))
	{

		DeafaultWeapon->SetWeaponCollisionEnable(true);
	}
	if (NotifyName == FName("UnableCollision"))
	{
		DeafaultWeapon->SetWeaponCollisionEnable(false);
	}
}
/* 通用 */
bool AEnemy::PlaySectionMontage(FName Section, UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance) || !IsValid(Montage)) return false;
	if(AnimInstance->Montage_Play(Montage) <= 0.f) return false;
	AnimInstance->Montage_JumpToSection(Section, Montage);
	return true;
}






