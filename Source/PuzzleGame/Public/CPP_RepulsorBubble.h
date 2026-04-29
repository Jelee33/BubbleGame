// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleGame/CPP_MainBubble.h"
#include "CPP_RepulsorBubble.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API ACPP_RepulsorBubble : public ACPP_MainBubble
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_RepulsorBubble();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
 
	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
 
	UPROPERTY(EditAnywhere)
	float MinLaunchForce = 600;
	


	
	
};
