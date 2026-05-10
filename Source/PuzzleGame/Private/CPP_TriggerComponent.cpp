// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_TriggerComponent.h"

// Sets default values for this component's properties
UCPP_TriggerComponent::UCPP_TriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	
}


// Called when the game starts
void UCPP_TriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MoverActor)
	{
		MoverComponent = MoverActor->FindComponentByClass<UCPP_MoverComponent>();
		
		if (!MoverComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Objeye MoverComponenti atamazsan nasil hareket ettiricez fatihcim?"));
		}
	}
	else if (MoverActor == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Mover Actor atamamissin fatihcim neyi hareket ettiricez? bu objete mover actor ata %s"),*GetOwner()->GetName());
	}
	
	if (Spawner)
	{
		SpawnerComponent = Spawner->FindComponentByClass<UCPP_SpawnerComponent>();
		
		if (!SpawnerComponent)
		{
			UE_LOG(LogTemp,Warning,TEXT("Spawner Component atamamissin spawner objesine"));
		}
	}
	else if (!Spawner)
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawner Actor atamamissin fatihcim neyi spawnlicaz? bu objeye spawner ata %s"),*GetOwner()->GetName());
	}
	
	
	OnComponentBeginOverlap.AddDynamic(this,&UCPP_TriggerComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UCPP_TriggerComponent::OnOverlapEnd);
	
}


// Called every frame
void UCPP_TriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UCPP_TriggerComponent::TriggerFunction(bool NewTriggerValue)
{
	
	if (IsTriggered == NewTriggerValue) return;
	
	IsTriggered = NewTriggerValue;
	
		if (MoverComponent)
		{
			MoverComponent->SetShouldMove(IsTriggered);
		}
	
		if (SpawnerComponent && IsTriggered)
		{
			SpawnerComponent->TriggerSpawn();
		}
	
}



void UCPP_TriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	ActivatorCount++;
	
	if (!IsTriggered)
	{
		TriggerFunction(true);
	}
}

void UCPP_TriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ActivatorCount = FMath::Max(0, ActivatorCount - 1);
	
	if (IsTriggered && ActivatorCount == 0)
	{
		TriggerFunction(false);
	}
}





