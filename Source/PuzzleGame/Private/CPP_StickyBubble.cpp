// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_StickyBubble.h"

#include "AssetTypeCategories.h"

ACPP_StickyBubble::ACPP_StickyBubble()
{
}

void ACPP_StickyBubble::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACPP_StickyBubble::OverlapBegin);
}

void ACPP_StickyBubble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_StickyBubble::OverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!IsAttached) return;
	if (HasCapturedObject) return;

	if (!IsValid(OtherActor) || !IsValid(OtherComp)) return;
	if (OtherActor == this || OtherActor == GetOwner()) return;

	// Bubble'ın yapıştığı yüzeyi yakalama.
	if (OtherActor == AttachedToActor || OtherComp == AttachedToComp) return;

	// Bubbleları yakalama.
	if (OtherActor->IsA(ACPP_MainBubble::StaticClass())) return;

	// Sadece physics objeleri yakala.
	if (!OtherComp->IsSimulatingPhysics()) return;

	if (ACPP_Bob* Bob = Cast<ACPP_Bob>(GetOwner()))
	{
		if (Bob->IsHolding && Bob->PhysicsHandleComponent &&
			Bob->PhysicsHandleComponent->GetGrabbedComponent() == OtherComp)
		{
			Bob->PhysicsHandleComponent->ReleaseComponent();
			Bob->IsHolding = false;
		}
	}

	OtherComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
	OtherComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	OtherComp->SetSimulatePhysics(false);

	if (USceneComponent* Root = OtherActor->GetRootComponent())
	{
		Root->SetUsingAbsoluteScale(true);
	}

	OtherActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	OtherComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	HasCapturedObject = true;

	UE_LOG(LogTemp, Warning, TEXT("Sticky captured: %s"), *OtherActor->GetName());
}
