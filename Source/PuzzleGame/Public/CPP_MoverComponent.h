// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_MoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUZZLEGAME_API UCPP_MoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCPP_MoverComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	UPROPERTY(EditAnywhere, Category="Settings")
	FVector MoveOffset;
	
	UPROPERTY(EditAnywhere, Category="Settings")
	FVector  StartLocation;
	
	UPROPERTY(EditAnywhere, Category="Settings")
	FVector  TargetLocation;
	
	UPROPERTY(EditAnywhere, Category="Settings")
	float MoveTime;
	
	
	float Speed;
	
	bool ReachedTarget;
	
	bool GetShouldMove();
	
	void SetShouldMove(bool NewShouldMove);
	
	
private:
	bool ShouldMove;
	
		
};
