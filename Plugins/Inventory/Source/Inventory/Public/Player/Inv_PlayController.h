

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Inv_PlayController.generated.h"


class UInputMappingContext;
class UInputAction;
class UInv_HUDWidget;
class UPlayerHeathBarWidget;


UCLASS()
class INVENTORY_API AInv_PlayController : public APlayerController
{
	GENERATED_BODY()

public:
	AInv_PlayController();

	virtual void Tick(float DeltaTime) override;



protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

private:

/* 追踪 */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	double TraceLength;
	//反射枚举
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;
	//Actor 被销毁，引擎后台自动检测，并把这个弱指针标记失效,强指针会锁住这个 Actor，垃圾回收不能清理。物体在画面上消失了，内存却一直残留。
	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;


	void TraceForItem();

/* 输入 */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	UInputMappingContext* IMC_Tianji;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputMappingContext> IMC_Inventory;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	void PrimaryInteract();
/* HUD */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;
	UPROPERTY()
	TObjectPtr< UInv_HUDWidget> HUDWidget;

	void CreateHUDWidget();
};
