// Fill out your copyright notice in the Description page of Project Settings.


#include "DDGASBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "DDAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "DDPushAwayGameplayEffect.h"
#include "DDDamageGameplayEffect.h"
#include "GameplayTagContainer.h"


ADDGASBaseCharacter::ADDGASBaseCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");

	AttributeSet = CreateDefaultSubobject<UDDAttributeSet>(TEXT("AttributeSet"));

    GASPlayerState = EGASPlayerState::Idle;
}


void ADDGASBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMoveSpeedAttribute()).AddUObject(this, &ADDGASBaseCharacter::OnMovementAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ADDGASBaseCharacter::OnHealthAttributeChanged);

	for (const TPair<EGASInputActions, TSubclassOf<UGameplayAbility>>& Ability : StandartAbilities)
    {
   		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability.Value, 1, static_cast<int32>(Ability.Key)));
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

    OnHealthChanged.Broadcast(AttributeSet->GetHealth());

    if (AttributeSet->GetHealth() <= 0)
    {
        GASPlayerState = EGASPlayerState::Dead;
        this->SetLifeSpan(1);
    }
}


void ADDGASBaseCharacter::OnGameplayEffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
    if (const UDDPushAwayGameplayEffect* PushbackEffect = Cast<UDDPushAwayGameplayEffect>(Spec.Def))
	{
        if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
        {
            AActor* EffectInstigator = Spec.GetEffectContext().GetInstigator();
            FVector Direction = EffectInstigator ? (GetActorLocation() - EffectInstigator->GetActorLocation()).GetSafeNormal2D() : GetActorForwardVector() * -1.0f;

            FRootMotionSource_ConstantForce* ConstantForce = new FRootMotionSource_ConstantForce();
            ConstantForce->AccumulateMode = ERootMotionAccumulateMode::Additive;
            ConstantForce->Force = Direction * PushbackEffect->Strength;
            ConstantForce->Duration = Spec.GetDuration();
            ConstantForce->FinishVelocityParams.Mode = ERootMotionFinishVelocityMode::SetVelocity;
            ConstantForce->FinishVelocityParams.SetVelocity = FVector::ZeroVector;

            TSharedRef<FRootMotionSource_ConstantForce> ConstantForceReference(ConstantForce);
            MovementComponent->ApplyRootMotionSource(ConstantForceReference);
        }
	}
    else if (const UDDDamageGameplayEffect* DamageEffect = Cast<UDDDamageGameplayEffect>(Spec.Def))
    {
        IsDamagedDuringStun = true;
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
        IsDamagedDuringStun = false;
    }
}