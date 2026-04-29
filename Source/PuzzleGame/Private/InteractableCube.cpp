// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableCube.h"

#include "Misc/MapErrors.h"

// Sets default values
AInteractableCube::AInteractableCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	RootComponent = BoxComp;
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void AInteractableCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractableCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

