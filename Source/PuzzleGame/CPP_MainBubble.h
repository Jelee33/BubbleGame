// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BubbleSettings.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CPP_MainBubble.generated.h"

UCLASS()
class PUZZLEGAME_API ACPP_MainBubble : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_MainBubble();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* VisualMesh;
	
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USphereComponent* Sphere;
	
	UPROPERTY(EditAnywhere)
	float SphereRadius = 500.f;
	
	UPROPERTY(EditAnywhere)
	float StartScale = 0.3f;
	
	UPROPERTY(EditAnywhere)
	float EndScale = 1.1f;
	
	UPROPERTY(EditAnywhere)
	float GrowSpeed = 2.0f;
	
	float CurrentScale = StartScale;
	bool IsGrowing;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UBubbleSettings* BubbleData;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynMat = nullptr;

	int32 MaxBubble =8;
	
	bool IsAttached;
	
	FVector SavedSurfaceNormal;
	

	
	
	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void OnDetonate();
	
	UPROPERTY(EditAnywhere)
	float LaunchPower = 500;
	private:

	


};
