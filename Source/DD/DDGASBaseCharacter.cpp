// Fill out your copyright notice in the Description page of Project Settings.


#include "DDGASBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "DDAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "GameplayPrediction.h"
#include "GameplayCueInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayTasksComponent.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Abilities/GameplayAbility.h"
#include "DDAbilitySystemComponent.h"


ADDGASBaseCharacter::ADDGASBaseCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDDAbilitySystemComponent>("AbilitySystemComponent");

	AttributeSet = CreateDefaultSubobject<UDDAttributeSet>(TEXT("AttributeSet"));
}


void ADDGASBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMoveSpeedAttribute()).AddUObject(this, &ADDGASBaseCharacter::OnMovementAttributeChanged);

	for (const auto& ability : StandartAbilities)
    {
   		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ability.Value, 1, static_cast<int32>(ability.Key)));
    }

	// AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ADDGASBaseCharacter::OnGameplayEffectApplied);
}


void ADDGASBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("EGASInputActions")));
}


UAbilitySystemComponent* ADDGASBaseCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}


void ADDGASBaseCharacter::OnMovementAttributeChanged(const FOnAttributeChangeData& Data)
{
    GetCharacterMovement()->MaxWalkSpeed = AttributeSet->GetMoveSpeed();
}


void ADDGASBaseCharacter::OnGameplayEffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	UE_LOG(LogTemp, Warning, TEXT("OnGameplayEffectApplied"));
}