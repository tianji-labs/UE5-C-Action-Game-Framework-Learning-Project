// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combat.h"
#include "AnimInstance/TianjiAnimInstance.h"
#include "Character/StateType.h"
#include "Character/Tianji.h"


UCombat::UCombat()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombat::BeginPlay()
{
	Super::BeginPlay();

/*监听通知*/
	Owner = GetOwner();
	ACharacter* CharOwner = Cast<ACharacter>(Owner);

	if (!IsValid(CharOwner) || !IsValid(CharOwner->GetMesh())) return;
	UAnimInstance* AnimInstance = CharOwner->GetMesh()->GetAnimInstance();

	if (!IsValid(AnimInstance)) return;
	AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UCombat::AttackEnd); //AddUniqueDynamic避免重复绑定

}

void UCombat::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//AttackEnd解绑
	ACharacter* CharOwner = Cast<ACharacter>(Owner);
	//不要用return跳过销毁
	if (IsValid(CharOwner) && IsValid(CharOwner->GetMesh()))
	{
		UAnimInstance* AnimInstance = CharOwner->GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance))
		{
			AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UCombat::AttackEnd);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UCombat::AttackEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	ATianji* OwnerCharacter = Cast<ATianji>(GetOwner());
	if (!IsValid(OwnerCharacter)) return;
	//引用公开接口——后续可换为接口或广播
	OwnerCharacter->SetActionState(EActionState::EAS_Unoccupied);
}
