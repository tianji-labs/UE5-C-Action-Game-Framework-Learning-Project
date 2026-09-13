// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon.h"
#include"Character/Tianji.h"
#include"Components/SphereComponent.h"
#include"Components/BoxComponent.h"
//数学运算
#include"Kismet/KismetSystemLibrary.h"
//受击接口
#include"Interface/GetHitInterface.h"
//受伤
#include"Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(MeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(MeshComponent);
/* 攻击重叠 */
	//攻击
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	//重叠事件
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxBeginOverlap);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeapon::BoxTrace(FHitResult& HitResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> IgnoreActor;
	IgnoreActor.AddUnique(this);
	IgnoreActor.AddUnique(GetOwner());

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoreActor,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true
	);
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	AActor* GetHitActor = BoxHit.GetActor();
	if (!(GetHitActor && GetHitActor->Implements<UGetHitInterface>())) return;
	IGetHitInterface::Execute_GetHit(GetHitActor, BoxHit.ImpactPoint, GetOwner());

}

//重叠事件
void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATianji* Tianji = Cast<ATianji>(OtherActor);
	if (Tianji)
	{
		Tianji->SetOverlappingItem(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ATianji* Tianji = Cast<ATianji>(OtherActor);
	if (Tianji)
	{
		Tianji->SetOverlappingItem(nullptr);
	}
}

void AWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FHitResult HitResult;
	BoxTrace(HitResult);
	ExecuteGetHit(HitResult);
	
	if (!IsValid(HitResult.GetActor()) || !IsValid(GetInstigator())) return;
	UGameplayStatics::ApplyDamage(HitResult.GetActor(), 20.f, GetInstigator()->GetController(), this, UDamageType::StaticClass());
}

//拾取武器
void AWeapon::AttachWeapon(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	//拾取
	AttachMeshToSocket(InParent, InSocketName);
	//拥有者与发起者
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	if (MeshComponent)
	{
		MeshComponent->AttachToComponent(InParent, TransformRules, InSocketName);
	}
	
}

void AWeapon::SetSphereNocollision()
{
	if (SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
