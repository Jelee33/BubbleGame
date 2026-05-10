// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_TriggerComponent.h"
#include "GameFramework/Actor.h"
#include "ILaserInteractable.h"
#include "CPP_Button.generated.h"

UCLASS()
class PUZZLEGAME_API ACPP_Button : public AActor, public ILaserInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Button();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UCPP_TriggerComponent* TriggerComponent;
	
	
	bool IsLaserHit = false;
	
	virtual void OnLaserHit_Implementation(FVector HitLocation, FVector HitNormal, class ACPP_Laser* LaserActor) override;
	
	virtual void OnLaserEnd_Implementation(class ACPP_Laser* LaserActor) override;
	
	virtual bool ShouldReflectLaser_Implementation() override;
	
	

};
