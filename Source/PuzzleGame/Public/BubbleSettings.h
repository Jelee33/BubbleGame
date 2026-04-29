// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BubbleSettings.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEGAME_API UBubbleSettings : public UDataAsset
{
	GENERATED_BODY()
	
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SphereRadiusDATA = 300.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaunchPowerDATA = 300.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxBubbleDATA = 8;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GrowSpeedDATA;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StartScaleDATA;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EndScaleDATA;
};
