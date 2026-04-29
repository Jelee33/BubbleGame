// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EAmmoType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EBProjectileBubbleType : uint8
{
	Sticky UMETA(DisplayName = "Sticky"),
	Repulsor UMETA(DisplayName = "Repulsor"),
	
};
