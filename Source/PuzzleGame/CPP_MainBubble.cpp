// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_MainBubble.h"

#include <filesystem>

#include "BoundShaderStateCache.h"
#include "BubbleSettings.h"
#include "CPP_Bob.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ACPP_MainBubble::ACPP_MainBubble()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = Sphere;
	
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>("VisualMesh"); 
	VisualMesh->SetupAttachment(Sphere);
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	

}

// Called when the game starts or when spawned
void ACPP_MainBubble::BeginPlay()
{
	Super::BeginPlay();
	
	if (Sphere)
	{
		Sphere->OnComponentHit.AddDynamic(this, &ACPP_MainBubble::OnHit);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("This component doesn't have a Sphere"));
	}
	
	IsAttached = false;
	
	if (BubbleData)
	{
		SphereRadius = BubbleData->SphereRadiusDATA;
		LaunchPower = BubbleData->LaunchPowerDATA;
		GrowSpeed = BubbleData->GrowSpeedDATA;
		StartScale = BubbleData->StartScaleDATA; 
		EndScale = BubbleData->EndScaleDATA;
	}
	
	SetActorScale3D(FVector(StartScale));

}

// Called every frame
void ACPP_MainBubble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsGrowing)
	{
		CurrentScale = FMath::FInterpConstantTo(CurrentScale,EndScale,DeltaTime,GrowSpeed);
		SetActorScale3D(FVector(CurrentScale));
		
		if (FMath::IsNearlyEqual(CurrentScale,EndScale,0.01f))
		{
			CurrentScale = EndScale;
			SetActorScale3D(FVector(EndScale));
			IsGrowing = false;
			SetActorTickEnabled(false);
		}
	}

}

void ACPP_MainBubble::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor) return;
	
	if (OtherActor->IsA(ACPP_MainBubble::StaticClass()))
	{
		
		ACPP_MainBubble* OtherBubble = Cast<ACPP_MainBubble>(OtherActor);
		if (!IsAttached && OtherBubble && OtherBubble->IsAttached) // yapisan balona atilan mermi patlayacak
		{
			if (ACPP_Bob* Bob =  Cast<ACPP_Bob>(GetOwner()))
			{
				Bob->ActiveBubbles.Remove(this);
			}
			this->Destroy();
			return;
		}

	}
	
	if (IsAttached || !OtherActor || OtherActor == GetOwner() || OtherActor == this ) return;
	
	IsAttached = true;
	
	SavedSurfaceNormal = Hit.Normal;
	
	IsGrowing = true;
	SetActorTickEnabled(true);
	
	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->Deactivate();
		ProjectileMovement->SetComponentTickEnabled(false);
	}
	
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
		Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		Sphere->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
		Sphere->UpdateOverlaps();
	}
	


	this->AttachToComponent(OtherComp,FAttachmentTransformRules::KeepWorldTransform);

	
}

void ACPP_MainBubble::OnDetonate()
{	
	bool CanDetonate = IsAttached;
	
	// IsAttached false ise return
	if (!CanDetonate) return;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	

	TArray<AActor*> OutActors;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	
	FVector ExplosionOrigin = GetActorLocation();
	if (bool bSphereOverlap = UKismetSystemLibrary::SphereOverlapActors(GetWorld(),GetActorLocation(),SphereRadius,ObjectTypes,nullptr,IgnoreActors,OutActors))
	{
		
		for (AActor* Actor : OutActors)
		{
			if (Actor->IsA<ACPP_MainBubble>()) continue;
			
			if (ACPP_Bob* Bob = Cast<ACPP_Bob>(Actor))
			{
					 
				FVector Distance = Bob->GetActorLocation() - ExplosionOrigin;
				FVector LaunchVelocity = Distance.GetSafeNormal() * LaunchPower;
				Bob->LaunchCharacter(LaunchVelocity,true,true);
			}
			
			else if (UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
			{
				if (Actor->GetAttachParentActor() == this || this->GetAttachParentActor() == Actor)
				{
					Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					UE_LOG(LogTemp,Warning,TEXT("Actor DetachFromActor"));
					RootComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					RootComp->SetSimulatePhysics(true);
					RootComp->RecreatePhysicsState();
					RootComp->WakeAllRigidBodies();
				}

				if (RootComp->IsSimulatingPhysics())
					
				{
					FVector Distance = RootComp->GetComponentLocation() - ExplosionOrigin;
					FVector LaunchVelocity = Distance.GetSafeNormal() * LaunchPower;
					RootComp->AddImpulse(LaunchVelocity, NAME_None, true);
				}
				
			}
		}
	}
	
	if (ACPP_Bob* Bob = Cast<ACPP_Bob>(GetOwner()))
	{
		Bob->ActiveBubbles.Remove(this);
	}
	Destroy();
	
}



