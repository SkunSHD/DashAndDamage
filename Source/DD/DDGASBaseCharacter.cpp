// Fill out your copyright notice in the Description page of Project Settings.


#include "DDGASBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "DDAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "DDPushAwayGameplayEffect.h"
#include "GameplayTagContainer.h"
#include "DDAbilitySystemComponent.h"


ADDGASBaseCharacter::ADDGASBaseCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDDAbilitySystemComponent>("AbilitySystemComponent");

	AttributeSet = CreateDefaultSubobject<UDDAttributeSet>(TEXT("AttributeSet"));

    GASPlayerState = EGASPlayerState::Idle;
}


void ADDGASBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMoveSpeedAttribute()).AddUObject(this, &ADDGASBaseCharacter::OnMovementAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ADDGASBaseCharacter::OnHealthAttributeChanged);

	for (const auto& ability : StandartAbilities)
    {
   		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ability.Value, 1, static_cast<int32>(ability.Key)));
    }

	AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ADDGASBaseCharacter::OnGameplayEffectApplied);
    AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Effect.Stun")).AddUObject(this, &ADDGASBaseCharacter::OnStunTagChanged);
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


void ADDGASBaseCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
    UE_LOG(LogTemp, Warning, TEXT("OnHealthAttributeChanged Health %f"), AttributeSet->GetHealth());

    if (AttributeSet->GetHealth() <= 0)
    {
        GASPlayerState = EGASPlayerState::Dead;
        this->SetLifeSpan(1);
    }
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

            TSharedRef<FRootMotionSource_ConstantForce> ConstantForceReference(constantForce);
            movementComponent->ApplyRootMotionSource(ConstantForceReference);
        }
	}
}


void ADDGASBaseCharacter::OnStunTagChanged(const FGameplayTag Tag, int32 Count)
{
    if (Count > 0)
    {
        GASPlayerState = EGASPlayerState::Stunned;
    }
    else
    {
        GASPlayerState = EGASPlayerState::Idle;
    }
}