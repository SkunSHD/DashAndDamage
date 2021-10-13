// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DDGASBaseCharacter.generated.h"


UENUM(BlueprintType)
enum class EGASInputActions : uint8
{
    None,
    Dash
};


UCLASS()
class DD_API ADDGASBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADDGASBaseCharacter();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditDefaultsOnly)
    TMap<EGASInputActions, TSubclassOf<class UGameplayAbility>> StandartAbilities;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
    class UDDAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
    class UDDAttributeSet* AttributeSet;

	void OnMovementAttributeChanged(const struct FOnAttributeChangeData& Data);

	// UFUNCTION()
	void OnGameplayEffectApplied(UAbilitySystemComponent* Source, const struct FGameplayEffectSpec& Spec, struct FActiveGameplayEffectHandle Handle);

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
