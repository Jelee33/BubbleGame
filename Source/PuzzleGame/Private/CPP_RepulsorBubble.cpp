// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_RepulsorBubble.h"

#include "MaterialHLSLTree.h"
#include "SceneRenderTargetParameters.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


ACPP_RepulsorBubble::ACPP_RepulsorBubble()
{
}

void ACPP_RepulsorBubble::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ACPP_RepulsorBubble::OverlapBegin);
	
	Sphere->OnComponentEndOverlap.AddDynamic(this,&ACPP_RepulsorBubble::OverlapEnd);


	
}

void ACPP_RepulsorBubble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_RepulsorBubble::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (!IsAttached || OtherActor == nullptr || OtherActor == GetAttachParentActor())
	{
		return;
	}

	if (IsAttached)
	{
		FVector InComingVelocity = OtherActor->GetVelocity();
		
		float MaxVelocity = FMath::Max3(FMath::Abs(InComingVelocity.X),FMath::Abs(InComingVelocity.Y),FMath::Abs(InComingVelocity.Z));

		
		FVector HitNormal = SavedSurfaceNormal;
		FVector LaunchForce =  HitNormal * FMath::Max(MaxVelocity, MinLaunchForce);
		
		
		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (OtherActor == Player)
		{
			Player->LaunchCharacter(LaunchForce,true,true );
			
		}
		else if (OtherComp && OtherComp->IsSimulatingPhysics())
		{


			
			OtherComp->SetPhysicsLinearVelocity(LaunchForce);

			
		}
		else
		{
			AActor* AttachedActor = GetAttachParentActor();

			if (AttachedActor && AttachedActor != OtherActor)
			{
				UPrimitiveComponent* AttachedPrimitive = Cast<UPrimitiveComponent>(AttachedActor->GetRootComponent());

				if (!AttachedPrimitive || !AttachedPrimitive->IsSimulatingPhysics())
				{
					TArray<UPrimitiveComponent*> PrimitiveComponents;
					AttachedActor->GetComponents<UPrimitiveComponent>(PrimitiveComponents);

					for (UPrimitiveComponent* Prim : PrimitiveComponents)
					{
						if (Prim && Prim->IsSimulatingPhysics())
						{
							AttachedPrimitive = Prim;
							break;
						}
					}
				}

				if (AttachedPrimitive && AttachedPrimitive->IsSimulatingPhysics())
				{
					FVector AttachedLaunchDirection = -SavedSurfaceNormal.GetSafeNormal();

					if (AttachedLaunchDirection.IsNearlyZero())
					{
						AttachedLaunchDirection = FVector::UpVector;
					}

					AttachedLaunchDirection.Z = FMath::Max(AttachedLaunchDirection.Z, 0.45f);
					AttachedLaunchDirection = AttachedLaunchDirection.GetSafeNormal();

					FVector AttachedLaunchForce = AttachedLaunchDirection * FMath::Max(MaxVelocity, MinLaunchForce);

					AttachedPrimitive->SetPhysicsLinearVelocity(AttachedLaunchForce, false);

					UE_LOG(LogTemp, Warning, TEXT("Attached physics actor launched: %s"), *AttachedActor->GetName());
				}
			}
		}
		
		

	}
	
}

void ACPP_RepulsorBubble::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}



