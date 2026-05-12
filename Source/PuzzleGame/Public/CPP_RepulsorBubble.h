#pragma once

#include "CoreMinimal.h"
#include "PuzzleGame/CPP_MainBubble.h"
#include "CPP_RepulsorBubble.generated.h"

UCLASS()
class PUZZLEGAME_API ACPP_RepulsorBubble : public ACPP_MainBubble
{
	GENERATED_BODY()
	
public:
	ACPP_RepulsorBubble();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
 
	UFUNCTION()
	void OverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Repulsor")
	float MinLaunchForce = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Repulsor")
	float AttachedCubeBounceForce = 1400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Repulsor")
	float SurfaceCheckExtraDistance = 20.f;

private:
	bool bWasTouchingExternalSurface = false;

	void CheckAttachedPhysicsSurfaceBounce();
};