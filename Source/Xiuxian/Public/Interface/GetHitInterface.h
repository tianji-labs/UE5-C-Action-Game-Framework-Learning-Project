

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "GetHitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGetHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class XIUXIAN_API IGetHitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint, AActor* Hitter);
};
