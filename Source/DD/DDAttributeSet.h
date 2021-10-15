// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DDAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)


UCLASS()
class DD_API UDDAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData Health = 100.0f;
    ATTRIBUTE_ACCESSORS(UDDAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly)
    FGameplayAttributeData MoveSpeed = 600.0f;
    ATTRIBUTE_ACCESSORS(UDDAttributeSet, MoveSpeed)

};
