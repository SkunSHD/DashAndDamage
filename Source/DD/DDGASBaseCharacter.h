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

UENUM(BlueprintType)
enum class EGASPlayerState : uint8
{
	Idle,
	Stunned,
	Dead
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

	UPROPERTY(BlueprintReadOnly)
	EGASPlayerState GASPlayerState;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
    class UDDAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
    class UDDAttributeSet* AttributeSet;

	void OnMovementAttributeChanged(const struct FOnAttributeChangeData& Data);

	void OnHealthAttributeChanged(const struct FOnAttributeChangeData& Data);

	// TODO: Figure out why build is crashed if this macro enabled
	// UFUNCTION()
	void OnGameplayEffectApplied(UAbilitySystemComponent* Source, const struct FGameplayEffectSpec& Spec, struct FActiveGameplayEffectHandle Handle);

	// TODO: Figure out why build is crashed if this macro enabled
	// UFUNCTION()
	void OnStunTagChanged(const struct FGameplayTag Tag, int32 Count);

	UPROPERTY(BlueprintReadOnly)
	bool IsDamagedDuringStun;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
