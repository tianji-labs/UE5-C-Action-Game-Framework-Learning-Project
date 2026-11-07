

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryInteractionComponent.generated.h"

class UInputMappingContext;
class UInputAction;
class UInv_HUDWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORY_API UInventoryInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<APlayerController> PlayerController;

/* 输入 */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputMappingContext> IMC_Inventory;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UInputAction> PrimaryInteractAction; 

	void PrimaryInteract();


/* 射线检测 */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	double TraceLength;
	//反射枚举
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;
	//Actor 被销毁，引擎后台自动检测，并把这个弱指针标记失效,强指针会锁住这个 Actor，垃圾回收不能清理。物体在画面上消失了，内存却一直残留。
	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;

	void TraceForItem();

/* HUD */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;
	UPROPERTY()
	TObjectPtr< UInv_HUDWidget> HUDWidget;

	void CreateHUDWidget();
public:	
/* 输入绑定 */
	void BindInput(UEnhancedInputComponent* EnhancedInputComponent);
		
	
};
