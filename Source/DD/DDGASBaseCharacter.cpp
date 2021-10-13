// Fill out your copyright notice in the Description page of Project Settings.


#include "DDGASBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "DDAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "DDPushAwayGameplayEffect.h"
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

	AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ADDGASBaseCharacter::OnGameplayEffectApplied);
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
	if (auto pushbackEffect = Cast<UDDPushAwayGameplayEffect>(Spec.Def))
	{
        if (auto movementComponent = GetCharacterMovement())
        {
            auto instigator = Spec.GetEffectContext().GetInstigator();
            auto direction = instigator ? (GetActorLocation() - instigator->GetActorLocation()).GetSafeNormal2D() : GetActorForwardVector() * -1.0f;

            auto constantForce = new FRootMotionSource_ConstantForce();
            constantForce->AccumulateMode = ERootMotionAccumulateMode::Additive;
            constantForce->Force = direction * pushbackEffect->Strength;
            constantForce->Duration = Spec.GetDuration();
            constantForce->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::SetVelocity;
            constantForce->FinishVelocityParams.SetVelocity = FVector::ZeroVector;
            movementComponent->ApplyRootMotionSource(constantForce);
        }
	}
}