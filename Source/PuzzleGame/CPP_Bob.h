// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "CPP_MainBubble.h"
#include "EAmmoType.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "CPP_Bob.generated.h"


UCLASS()
class PUZZLEGAME_API ACPP_Bob : public ACharacter
{
	GENERATED_BODY()

public:
	ACPP_Bob();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;
	
	APlayerController* PlayerController;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveObjectAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ChangeAmmoAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* FireAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DetonateAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RotateHeldObjectAction;
	
	UPROPERTY()
	TArray<ACPP_MainBubble*> ActiveBubbles;
	
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere)
	UPhysicsHandleComponent* PhysicsHandleComponent;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* HoldPoint;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACPP_MainBubble> StickyBubbleClass = nullptr;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACPP_MainBubble> RepulsorBubbleClass = nullptr;
	
	
	TSubclassOf<ACPP_MainBubble> CurrentBubbleClass = RepulsorBubbleClass;

	UPROPERTY(EditAnywhere)
	EBProjectileBubbleType CurrentBubbleType = EBProjectileBubbleType::Repulsor;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* GunMesh;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BubbleSpawnPoint;
	
	UPROPERTY(EditAnywhere)
	UBubbleSettings* BubbleSettings;
	
	UPROPERTY(EditAnywhere, Category="Object Hold")
	float HeldObjectRotationStep = 15.f;
	

	FRotator HeldObjectRotation;
	
	int32 MaxBubble =8;
	
 	bool IsHolding = false;
	
	bool CantUseGun();

	TArray<AActor*> AttachedActors;
	

	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	void JumpInput();
	void FireInput();
	void ChangeAmmoInput();
	void DetonateInput();
	void MoveObjectInput();
	void ReleaseHoldObject();
	void SetObjectLocationRotation();
	void RotateHeldObjectInput(const FInputActionValue& Value);

};
