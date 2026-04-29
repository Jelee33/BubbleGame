// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Bob.h"

#include "CPP_StickyBubble.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NetworkMessage.h"
#include "Chaos/AABBTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
// Sets default values
ACPP_Bob::ACPP_Bob()
{
	PrimaryActorTick.bCanEverTick = true;
	
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	
	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	
	HoldPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HoldPoint"));
	HoldPoint->SetupAttachment(Camera);
	
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(Camera);
	
	BubbleSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BubbleSpawnPoint"));
	BubbleSpawnPoint->SetupAttachment(GunMesh);
	
	

}

void ACPP_Bob::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(DefaultMappingContext,0);
			}
		}
	}
	
	if (BubbleSettings)
	{
		MaxBubble = BubbleSettings->MaxBubbleDATA;
	}
	
	
	CurrentBubbleClass = RepulsorBubbleClass;
	CurrentBubbleType = EBProjectileBubbleType::Repulsor;
	
	IsHolding = false;
}

void ACPP_Bob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetObjectLocationRotation();
	
	if (IsHolding)
	{
		if (PhysicsHandleComponent->GetGrabbedComponent())
		{
			FVector ObjectLocation = PhysicsHandleComponent->GetGrabbedComponent()->GetComponentLocation();
			float PlayerAndObjectDistance = (this->GetActorLocation() - ObjectLocation).Size();
			float PlayerAndHoldPointDistance = (this->GetActorLocation() - HoldPoint->GetComponentLocation()).Size();
		
			if ( PlayerAndObjectDistance > PlayerAndHoldPointDistance * 2)
			{
				MoveObjectInput();
			}
				
		}
		
	}
	
	


}

void ACPP_Bob::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_Bob::MoveInput);
		EnhancedInputComponent->BindAction(LookAction,ETriggerEvent::Triggered,this , &ACPP_Bob::LookInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACPP_Bob::JumpInput);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ACPP_Bob::FireInput);
		EnhancedInputComponent->BindAction(ChangeAmmoAction,ETriggerEvent::Started, this, &ACPP_Bob::ChangeAmmoInput);
		EnhancedInputComponent->BindAction(DetonateAction,ETriggerEvent::Started,this,&ACPP_Bob::DetonateInput);
		EnhancedInputComponent->BindAction(MoveObjectAction,ETriggerEvent::Started,this,&ACPP_Bob::MoveObjectInput);
	}

}

bool ACPP_Bob::CantUseGun()
{
	return IsHolding; // BIRSEY TUTUYORSAK TRUE DONER
}

void ACPP_Bob::MoveInput(const FInputActionValue& Value)
{
	
	if (!Controller) return;

	FVector2D InputValue = Value.Get<FVector2D>();
	FVector ActorRightVector = GetActorRightVector();
	FVector ActorForwardVector  = GetActorForwardVector();


	AddMovementInput(ActorRightVector,InputValue.Y);
	AddMovementInput(ActorForwardVector,InputValue.X);
}

void ACPP_Bob::LookInput(const FInputActionValue& Value)
{
	if (!Controller) return;
	
	FVector2D MouseAxis = Value.Get<FVector2D>();
	AddControllerPitchInput(MouseAxis.Y);
	AddControllerYawInput(-MouseAxis.X);
}

void ACPP_Bob::JumpInput()
{
	if (!Controller) return;
	
	Jump();

	
}

void ACPP_Bob::FireInput()
{

	if (CantUseGun()) return;
	
	 if (!Controller || !Camera || !BubbleSpawnPoint || !CurrentBubbleClass)
	 {
	 	UE_LOG(LogTemp,Warning,TEXT("KOK"));
		 return;
	 }



	
	FVector Start = Camera->GetComponentLocation();
	FVector End = Camera->GetForwardVector() * 50000.0f + Start;
	
	FVector SpawnLocation = BubbleSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = BubbleSpawnPoint->GetComponentRotation();
	

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	
	FHitResult HitResult;
	if (bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_Visibility,CollisionQueryParams))
	{
		if (ACPP_MainBubble* MainBubble = GetWorld()->SpawnActor<ACPP_MainBubble>(CurrentBubbleClass,SpawnLocation,SpawnRotation))
		{
			MainBubble->SetOwner(this);
			ActiveBubbles.Add(MainBubble);
		}
	}
	
	if (ActiveBubbles.Num() >= MaxBubble )
	{
		if (ActiveBubbles[0])
		{
			
			ActiveBubbles[0]->Destroy();
		}
		ActiveBubbles.RemoveAt(0);
	}
	
}

void ACPP_Bob::ChangeAmmoInput()
{
	switch (CurrentBubbleType)
	{
		case EBProjectileBubbleType::Repulsor:
			CurrentBubbleType = EBProjectileBubbleType::Sticky;
			UE_LOG(LogTemp,Warning,TEXT("Bubble Type %s"),*UEnum::GetValueAsString(CurrentBubbleType))
			break;
		case EBProjectileBubbleType::Sticky:
			CurrentBubbleType = EBProjectileBubbleType::Repulsor;
			UE_LOG(LogTemp,Warning,TEXT("Bubble Type %s"),*UEnum::GetValueAsString(CurrentBubbleType))
			break;
		
		default:
			break;
		
	}
	
	switch (CurrentBubbleType)
	{
	case EBProjectileBubbleType::Sticky:
		CurrentBubbleClass = StickyBubbleClass;
		break;
	case EBProjectileBubbleType::Repulsor:
		CurrentBubbleClass = RepulsorBubbleClass;
		break;
	default:
		break;
	}

}

void ACPP_Bob::DetonateInput()
{
	if (CantUseGun()) return;

	
	
	FVector Start = Camera->GetComponentLocation();
	FVector End = Camera->GetForwardVector() * 50000.0f + Start;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	
	TArray<FHitResult> HitResults;
	
	if (bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),Start,End,60.0f,ObjectTypes,false,IgnoreActors,EDrawDebugTrace::None,HitResults,true))
	{
		for (FHitResult& HitResult : HitResults)
		{
			if (ACPP_MainBubble* Bubble = Cast<ACPP_MainBubble>(HitResult.GetActor()))

			{
				Bubble->OnDetonate();
				
			}
		}
	}
}

void ACPP_Bob::MoveObjectInput()
{
	if (IsHolding)
	{
		UPrimitiveComponent* HoldingObject = PhysicsHandleComponent->GetGrabbedComponent();
		ReleaseHoldObject();		
		for (AActor* Actor : AttachedActors)
		{
			if (ACPP_MainBubble* Bubble = Cast<ACPP_MainBubble>(Actor))
			{
				if (Bubble->Sphere)
				{
					Bubble->Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
				}
			}
		}
		AttachedActors.Empty();
		
		IsHolding = false;
		
		if (HoldingObject)
		{
			HoldingObject->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
			
			HoldingObject->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
			
			FVector CurrentVelocity = HoldingObject->GetComponentVelocity();
			
			float MaxSpeed = 100.f;
			
			if (CurrentVelocity.Size() > MaxSpeed)
			{
				HoldingObject->SetPhysicsLinearVelocity(CurrentVelocity.GetSafeNormal() * MaxSpeed);
			}
			
			TArray<AActor*> NearbyStickyBubbles;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_StickyBubble::StaticClass(), NearbyStickyBubbles);
			for (AActor* Actor : NearbyStickyBubbles)
			{
				if (ACPP_StickyBubble* Sticky = Cast<ACPP_StickyBubble>(Actor))
				{
					Sticky->Sphere->UpdateOverlaps();
				}
			}
		}
	}
	else
	{
		FVector Start = Camera->GetComponentLocation();
		FVector End = Camera->GetForwardVector() * 500.0f + Start;
		FCollisionQueryParams ObjectParams;
		ObjectParams.AddIgnoredActor(this);
		FHitResult HitResult;

		if (GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_Visibility,ObjectParams))
		{
			
			UE_LOG(LogTemp,Warning,TEXT("%s"),*HitResult.GetComponent()->GetName());
			if(HitResult.GetComponent() && HitResult.GetComponent()->IsSimulatingPhysics())
			{
				AActor* Object = HitResult.GetActor();
				
				Object->GetAttachedActors(AttachedActors);
				
				for (AActor* Actor : AttachedActors)
				{
					if (ACPP_MainBubble* Bubble = Cast<ACPP_MainBubble>(Actor))
					{
						if (Bubble->Sphere)
						{
							Bubble->Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
						}
					}
				}
				UPrimitiveComponent* ObjectComponent = HitResult.GetComponent();
				
				ObjectComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);

				FVector ObjectLocation = Object->GetActorLocation();
				
				FRotator ObjectRotation = Object->GetActorRotation();
				
				PhysicsHandleComponent->GrabComponentAtLocationWithRotation(ObjectComponent,NAME_None,ObjectLocation,ObjectRotation);
				IsHolding = true;
				
			}
		}
	}
}

void ACPP_Bob::ReleaseHoldObject()
{
	if (PhysicsHandleComponent && PhysicsHandleComponent->GetGrabbedComponent())
	{
		PhysicsHandleComponent->ReleaseComponent();
	}
}

void ACPP_Bob::SetObjectLocationRotation()
{
	if (IsHolding)
	{
		PhysicsHandleComponent->SetTargetLocationAndRotation(HoldPoint->GetComponentLocation(),HoldPoint->GetComponentRotation());
	}
}
