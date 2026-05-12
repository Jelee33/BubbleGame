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
	
	if (!IsAttached || OtherActor == nullptr || OtherActor == AttachedToActor)
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
		else if (OtherComp->IsSimulatingPhysics())
		{


			
			OtherComp->SetPhysicsLinearVelocity(LaunchForce);

			
		}
		else
		{
			// Bubble küpe yapıştıysa ve bubble yere/duvara değdiyse,
			// OtherActor zemin olur. Fırlatmamız gereken şey zemin değil,
			// bubble'ın yapıştığı küptür.

			if (AttachedToComp && AttachedToComp->IsSimulatingPhysics())
			{
				FVector AttachedLaunchDirection = -SavedSurfaceNormal.GetSafeNormal();

				if (AttachedLaunchDirection.IsNearlyZero())
				{
					AttachedLaunchDirection = FVector::UpVector;
				}

				// Küpün altına yapışınca yukarı sektirmeyi garantiye al.
				AttachedLaunchDirection.Z = FMath::Max(AttachedLaunchDirection.Z, 0.55f);
				AttachedLaunchDirection = AttachedLaunchDirection.GetSafeNormal();

				FVector AttachedLaunchForce = AttachedLaunchDirection * FMath::Max(MaxVelocity, MinLaunchForce);

				AttachedToComp->SetPhysicsLinearVelocity(AttachedLaunchForce, false);

				UE_LOG(LogTemp, Warning, TEXT("Repulsor launched ATTACHED cube: %s Force: %s"),
					*AttachedToActor->GetName(),
					*AttachedLaunchForce.ToString());
			}
		}
		

	}
	
}

void ACPP_RepulsorBubble::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}



