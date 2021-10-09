// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DDGASBaseCharacter.generated.h"

struct FOnAttributeChangeData;

UCLASS()
class DD_API ADDGASBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADDGASBaseCharacter();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
    class UDDAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
    class UDDAttributeSet* AttributeSet;

	void OnMovementAttributeChanged(const FOnAttributeChangeData& Data);

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
