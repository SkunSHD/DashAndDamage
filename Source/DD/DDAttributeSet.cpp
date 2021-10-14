// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffectExtension.h"
#include "DDGASBaseCharacter.h"
#include "DDAttributeSet.h"


void UDDAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffectExecute Health %f"), Health.GetCurrentValue());
	if (Health.GetCurrentValue() <= 0) 
	{
		UE_LOG(LogTemp, Warning, TEXT("PostGameplayEffectExecute Death"));
		AActor* DamagedActor = nullptr;

		if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
		{
			DamagedActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
			DamagedActor->SetLifeSpan(1);
		}
	}
}