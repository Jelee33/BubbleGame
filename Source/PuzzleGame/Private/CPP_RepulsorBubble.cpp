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
		else if (OtherComp->IsSimulatingPhysics())
		{


			
			OtherComp->SetPhysicsLinearVelocity(LaunchForce);

			
		}
		
		

	}
	
}

void ACPP_RepulsorBubble::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}



