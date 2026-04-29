// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_MoverComponent.h"

// Sets default values for this component's properties
UCPP_MoverComponent::UCPP_MoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCPP_MoverComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	StartLocation = GetOwner()->GetActorLocation();
	SetShouldMove(false);
	Speed = MoveOffset.Size() / MoveTime;
	
}


// Called every frame
void UCPP_MoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentLocation = GetOwner()->GetActorLocation();
	float Distance = FVector::Dist(CurrentLocation, TargetLocation);
	ReachedTarget = Distance < 1.0f;
	
	if (!ReachedTarget)
	{
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation,TargetLocation,DeltaTime,Speed);
		FVector DeltaMoveOffset = NewLocation - CurrentLocation;
		GetOwner()->AddActorWorldOffset(DeltaMoveOffset,false);
	}
	
}

bool UCPP_MoverComponent::GetShouldMove()
{
	return ShouldMove;
}

void UCPP_MoverComponent::SetShouldMove(bool NewShouldMove)
{
	ShouldMove = NewShouldMove;
	
	if (ShouldMove)
	{
		TargetLocation = StartLocation + MoveOffset;
	}
	else
	{
		TargetLocation = StartLocation;
	}
}


