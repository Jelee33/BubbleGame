// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Button.h"

// Sets default values
ACPP_Button::ACPP_Button()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	

	
}

// Called when the game starts or when spawned
void ACPP_Button::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerComponent = FindComponentByClass<UCPP_TriggerComponent>();
	
}

// Called every frame
void ACPP_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	


	
}

void ACPP_Button::OnLaserHit_Implementation(FVector HitLocation, FVector HitNormal, class ACPP_Laser* LaserActor)
{
	if (IsLaserHit == true) return;
	
	if (TriggerComponent)
	{
		IsLaserHit = true;
		TriggerComponent->TriggerFunction(true);
		UE_LOG(LogTemp,Warning,TEXT("Laser degıyor"));

	}
		
	
	
	
	
}

void ACPP_Button::OnLaserEnd_Implementation(class ACPP_Laser* LaserActor)
{
	if (TriggerComponent)
	{
		TriggerComponent->TriggerFunction(false);
		IsLaserHit = false;

	}
}


bool ACPP_Button::ShouldReflectLaser_Implementation()
{
	return false;
}

