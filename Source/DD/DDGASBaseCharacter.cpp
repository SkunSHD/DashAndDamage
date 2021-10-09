// Fill out your copyright notice in the Description page of Project Settings.


#include "DDGASBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "DDAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
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