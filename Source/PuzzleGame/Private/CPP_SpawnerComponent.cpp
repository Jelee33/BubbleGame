// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_SpawnerComponent.h"

// Sets default values for this component's properties
UCPP_SpawnerComponent::UCPP_SpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCPP_SpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCPP_SpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCPP_SpawnerComponent::TriggerSpawn()
{
	
	if (!IsValid(ObjectToSpawn))
	{
		UE_LOG(LogTemp, Error, TEXT("SPAWN OLCAK OBJEYi ATAAA"));
		return;
	}
	
	if (IsValid(CurrentSpawnedObject))
	{
		CurrentSpawnedObject->Destroy();
	}
	
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;
	
	FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorForwardVector() * SpawnForwardOffset;
	FRotator SpawnRotation = Owner->GetActorRotation();
	
	CurrentSpawnedObject = GetWorld()->SpawnActor<AActor>(ObjectToSpawn,SpawnLocation, SpawnRotation);
}

