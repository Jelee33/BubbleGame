#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ILaserInteractable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class ULaserInteractable : public UInterface
{
	GENERATED_BODY()
};

class PUZZLEGAME_API ILaserInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Laser")
	void OnLaserHit(FVector HitLocation, FVector HitNormal, class ACPP_Laser* LaserActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Laser")
	void OnLaserEnd(class ACPP_Laser* LaserActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Laser")
	bool ShouldReflectLaser();
};