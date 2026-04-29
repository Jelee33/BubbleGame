// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include  "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "CPP_Laser.generated.h"

UCLASS()
class PUZZLEGAME_API ACPP_Laser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Laser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* LaserRoot;
	
	UPROPERTY(EditAnywhere)
	class USceneComponent* LaserOut;
	
	UPROPERTY(EditAnywhere,Category = "Laser Settings")
	float MaxDistance = 20000.f;
	
	UPROPERTY(EditAnywhere,Category = "Laser Settings")
	int32 MaxReflections = 8;
	

	
	UPROPERTY(EditAnywhere, Category = "Laser VFX")
	UNiagaraSystem* LaserBeamSystem;
	
	UPROPERTY(EditAnywhere, Category = "Laser VFX")
	UNiagaraSystem* LaserImpactSystem;
	
	UPROPERTY()
	TSet<AActor*> PreviousHitActors;

	UPROPERTY()
	TArray<UNiagaraComponent*> LaserSegments;
	
	TArray<FVector> LaserPoints;
	
	void Laser();
	void UpdateLaserVFX();
	
	int32 PreviusHitCount;
};
