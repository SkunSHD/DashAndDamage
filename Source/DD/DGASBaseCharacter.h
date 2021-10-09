// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DGASBaseCharacter.generated.h"

struct FOnAttributeChangeData;

UCLASS()
class DD_API ADGASBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ADGASBaseCharacter();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	UPROPERTY()
    class UDDAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
    class UDDAttributeSet* AttributeSet;

	void OnMovementAttributeChanged(const FOnAttributeChangeData& Data);

public:

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
