// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon.h"
#include"Character/Tianji.h"
#include"Components/SphereComponent.h"
#include"Components/BoxComponent.h"


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

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	//球体重叠事件
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


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
