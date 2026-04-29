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

void ACPP_StickyBubble::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (this->GetAttachParentActor() == OtherActor) return;
	if (!IsValid(OtherActor) || OtherActor == this) return;
	if (!IsAttached) return;
	
	
	if (ACPP_Bob* Bob = Cast<ACPP_Bob>(GetOwner()))
	{
		if (Bob->IsHolding && Bob->PhysicsHandleComponent->GetGrabbedComponent() == OtherComp)
		{
			Bob->PhysicsHandleComponent->ReleaseComponent();
			Bob->IsHolding = false;
			OtherComp->SetSimulatePhysics(true);

		}
	}
	
	if (OtherComp->IsSimulatingPhysics() && IsValid(OtherComp))
	{
		if (OtherComp->GetAttachParent() == GetRootComponent()) return;
		
		if (OtherActor->IsAttachedTo(this)) return;
		
		if (OtherComp->IsSimulatingPhysics())
		{
			OtherComp->SetSimulatePhysics(false);
			
			if (USceneComponent* CubeRoot = OtherActor->GetRootComponent())
			{
				CubeRoot->SetUsingAbsoluteScale(true); // balon buyuyunce kup buyumesin diye uygulanan bir ayar.
			}
			
			
			OtherActor->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
			OtherComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			OtherComp->RecreatePhysicsState(); // yapisinca mesh bozuluyordu hesaplamalari tekrar hesaplatiyoruz o yuzden
			OtherComp->WakeAllRigidBodies();
			IsAttached = true;
			
		}
	}

}
