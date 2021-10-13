// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "DDPushAwayGameplayEffect.generated.h"

/**
 * 
 */
UCLASS()
class DD_API UDDPushAwayGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	float Strength = 100;
};
