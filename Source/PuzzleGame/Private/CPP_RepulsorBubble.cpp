#include "CPP_RepulsorBubble.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ACPP_RepulsorBubble::ACPP_RepulsorBubble()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACPP_RepulsorBubble::BeginPlay()
{
	Super::BeginPlay();

	if (Sphere)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACPP_RepulsorBubble::OverlapBegin);
		Sphere->OnComponentEndOverlap.AddDynamic(this, &ACPP_RepulsorBubble::OverlapEnd);
	}
}

void ACPP_RepulsorBubble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// MainBubble Tick bazen işi bitince Tick'i kapatıyor.
	// Repulsor physics objeye yapıştıysa surface kontrolü için Tick açık kalmalı.
	if (IsAttached && AttachedToComp)
	{
		CheckAttachedPhysicsSurfaceBounce();
	}
}

void ACPP_RepulsorBubble::OverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!IsAttached || !OtherActor)
	{
		return;
	}

	// Bubble'ın yapıştığı physics objeye direkt overlap etkisi verme.
	// Havada küpe bubble atınca küpün uçmasını engelleyen ana satır bu.
	if (OtherActor == AttachedToActor)
	{
		return;
	}

	// Eğer bubble physics objeye yapıştıysa, normal overlap ile başka şeyleri zıplatmasın.
	// Çünkü bu modda görevi: dış yüzeye temas edince yapıştığı objeyi sektirmek.
	if (AttachedToComp && AttachedToComp->IsSimulatingPhysics())
	{
		return;
	}

	FVector LaunchDirection = SavedSurfaceNormal.GetSafeNormal();

	if (LaunchDirection.IsNearlyZero())
	{
		LaunchDirection = FVector::UpVector;
	}

	const float IncomingSpeed = OtherActor->GetVelocity().Size();
	const float FinalForce = FMath::Max(IncomingSpeed, MinLaunchForce);
	const FVector LaunchVelocity = LaunchDirection * FinalForce;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (OtherActor == Player)
	{
		Player->LaunchCharacter(LaunchVelocity, true, true);

		UE_LOG(LogTemp, Warning, TEXT("Repulsor launched player: %s"), *LaunchVelocity.ToString());
		return;
	}

	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->SetPhysicsLinearVelocity(LaunchVelocity, false);
		OtherComp->WakeAllRigidBodies();

		UE_LOG(LogTemp, Warning, TEXT("Repulsor launched physics actor: %s Force: %s"),
			*OtherActor->GetName(),
			*LaunchVelocity.ToString()
		);

		return;
	}
}

void ACPP_RepulsorBubble::OverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}

void ACPP_RepulsorBubble::CheckAttachedPhysicsSurfaceBounce()
{
	if (!IsAttached || !AttachedToComp)
	{
		return;
	}

	if (!AttachedToComp->IsSimulatingPhysics())
	{
		bWasTouchingExternalSurface = false;
		return;
	}

	if (!Sphere)
	{
		return;
	}

	FVector SurfaceDirection = SavedSurfaceNormal.GetSafeNormal();

	if (SurfaceDirection.IsNearlyZero())
	{
		return;
	}

	const float TraceDistance = Sphere->GetScaledSphereRadius() + SurfaceCheckExtraDistance;

	const FVector Start = GetActorLocation();
	const FVector End = Start + SurfaceDirection * TraceDistance;

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (AttachedToActor)
	{
		Params.AddIgnoredActor(AttachedToActor);
	}

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	const bool bHitExternalSurface = GetWorld()->LineTraceSingleByObjectType(
		Hit,
		Start,
		End,
		ObjectParams,
		Params
	);

	if (!bHitExternalSurface)
	{
		bWasTouchingExternalSurface = false;
		return;
	}

	if (bWasTouchingExternalSurface)
	{
		return;
	}

	bWasTouchingExternalSurface = true;

	FVector LaunchDirection = -SurfaceDirection;

	if (LaunchDirection.IsNearlyZero())
	{
		LaunchDirection = FVector::UpVector;
	}

	LaunchDirection.Z = FMath::Max(LaunchDirection.Z, 0.65f);
	LaunchDirection = LaunchDirection.GetSafeNormal();

	const FVector LaunchVelocity = LaunchDirection * AttachedCubeBounceForce;

	AttachedToComp->SetPhysicsLinearVelocity(LaunchVelocity, false);
	AttachedToComp->WakeAllRigidBodies();

	UE_LOG(LogTemp, Warning, TEXT("Repulsor bounced attached physics actor: %s Hit: %s Force: %s"),
		AttachedToActor ? *AttachedToActor->GetName() : TEXT("None"),
		Hit.GetActor() ? *Hit.GetActor()->GetName() : TEXT("None"),
		*LaunchVelocity.ToString()
	);
}