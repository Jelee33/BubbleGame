// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Laser.h"
#include "ILaserInteractable.h"

// Sets default values
ACPP_Laser::ACPP_Laser()
{
	PrimaryActorTick.bCanEverTick = true;
	
	
	LaserRoot = CreateDefaultSubobject<UStaticMeshComponent>("LaserVisual");
	SetRootComponent(LaserRoot);
	
	LaserOut = CreateDefaultSubobject<USceneComponent>("LaserPoint");
	LaserOut->SetupAttachment(LaserRoot);
	
	
	

}

// Called when the game starts or when spawned
void ACPP_Laser::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Laser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Laser();

}

void ACPP_Laser::Laser()
{
	LaserPoints.Empty();
	FVector CurrentStart = LaserOut->GetComponentLocation();
	FVector CurrentDirection = LaserOut->GetForwardVector();
	
	LaserPoints.Add(CurrentStart);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	TSet<AActor*> CurrentHitActors;
	
	for (int32 i = 0; i < MaxReflections; i++)
	{
		FVector End = CurrentStart + (CurrentDirection * MaxDistance);
		FHitResult OutHit;

		if (GetWorld()->LineTraceSingleByChannel(OutHit,CurrentStart,End,ECC_Visibility,Params))
		{
			LaserPoints.Add(OutHit.Location);

			AActor* HitActor = OutHit.GetActor();
			// Interfacenin oldugu objeler
			if (HitActor && HitActor->Implements<ULaserInteractable>())
			{
				CurrentHitActors.Add(HitActor);
				ILaserInteractable::Execute_OnLaserHit(HitActor,OutHit.Location, OutHit.Normal, this);
				
				// Interfacenin oldugu objeler ve lazerin yansitilmasina izin verilen objeler
				if (ILaserInteractable::Execute_ShouldReflectLaser(HitActor))
				{
					CurrentDirection = FMath::GetReflectionVector(CurrentDirection, OutHit.Normal);
					CurrentStart = OutHit.Location + (CurrentDirection * 1.1f);
					Params.AddIgnoredActor(HitActor);
					continue;
				}
				// Interfacenin oldugu ama lazerin yansitilmasina izin verilmeyen objeler trigger buraya eklenecek
				else
				{
					LaserPoints.Add(OutHit.Location);
					break;
				}
			}
			
			// Interfacenin olmadigi objeler (lazer kesilecek)
			else
			{
				LaserPoints.Add(OutHit.Location);
				break;
			}
		
		}
		else
		{
			LaserPoints.Add(End);
			break;
		}
		
	}
	
	for (AActor* Prev : PreviousHitActors)
	{
		if (IsValid(Prev) && !CurrentHitActors.Contains(Prev)) // PrevActor varsa ve  currentin icinde yer almiyorsa laser cekilmistir
		{
			if (Prev->Implements<ULaserInteractable>())
			{
				ILaserInteractable::Execute_OnLaserEnd(Prev,this);
			}
			
		}
	}
	
	PreviousHitActors = CurrentHitActors;
	
	
	UpdateLaserVFX();
}

void ACPP_Laser::UpdateLaserVFX()
{
	if (!LaserBeamSystem) return;
	
	
	int32 SegmentCount = LaserPoints.Num() - 1;

	while (LaserSegments.Num() < SegmentCount)
	{
		UNiagaraComponent* Comp = NewObject<UNiagaraComponent>(
			this,
			UNiagaraComponent::StaticClass(),
			NAME_None,
			RF_Transient
			);
		
		Comp->SetAsset(LaserBeamSystem);
		Comp->SetupAttachment(GetRootComponent());
		Comp->RegisterComponent();
		Comp->SetAutoActivate(false);
		LaserSegments.Add(Comp);
	}
	
	for (int32 i = 0; i < SegmentCount; i++)
	{	
	
		
		UNiagaraComponent* Segment = LaserSegments[i];
		
		
		
		Segment->SetVariablePosition(FName("User.BeamStartLocation"),LaserPoints[i]);
		Segment->SetVariablePosition(FName("User.BeamEndLocation"),LaserPoints[i+1]);
		
		if (!Segment->IsActive()) Segment->Activate(true);
	}
	
	for (int32 i = SegmentCount; i < LaserSegments.Num(); i++)
	{
		if (LaserSegments[i]->IsActive()) LaserSegments[i]->Deactivate();
	}
}

