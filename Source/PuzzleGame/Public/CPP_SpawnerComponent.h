// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_SpawnerComponent.generated.h"

class UCPP_TriggerComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLEGAME_API UCPP_SpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCPP_SpawnerComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere,Category = "Spawner Settings")
	TSubclassOf<AActor> ObjectToSpawn;
	
	UPROPERTY(EditAnywhere,Category = "Spawner Settings")
	float SpawnForwardOffset = 150.0f;
	
	UPROPERTY(EditAnywhere)
	UCPP_TriggerComponent* TriggerComponent;
	
	AActor* CurrentSpawnedObject = nullptr;
	
	UFUNCTION()
	void TriggerSpawn();
		
};
