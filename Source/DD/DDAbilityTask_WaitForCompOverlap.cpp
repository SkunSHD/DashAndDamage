// Fill out your copyright notice in the Description page of Project Settings.


#include "DDAbilityTask_WaitForCompOverlap.h"


void UDDAbilityTask_WaitForCompOverlap::Activate()
{
    Super::Activate();

    if (Component)
    {
        Component->OnComponentBeginOverlap.AddDynamic(this, &UDDAbilityTask_WaitForCompOverlap::OnHitCallback);
    }
}


void UDDAbilityTask_WaitForCompOverlap::OnDestroy(bool AbilityEnded)
{
    if (Component)
    {
        Component->OnComponentBeginOverlap.RemoveAll(this);
    }

    Super::OnDestroy(AbilityEnded);
}


UDDAbilityTask_WaitForCompOverlap* UDDAbilityTask_WaitForCompOverlap::WaitForComponentOverlap(UGameplayAbility* OwningAbility, UPrimitiveComponent* Component)
{
    auto abilityTask = NewAbilityTask<UDDAbilityTask_WaitForCompOverlap>(OwningAbility);
    abilityTask->Component = Component;
    return abilityTask;
}


void UDDAbilityTask_WaitForCompOverlap::OnHitCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        auto targetData = new FGameplayAbilityTargetData_ActorArray();
        targetData->TargetActorArray.Add(OtherActor);

        auto targetDataHandle = FGameplayAbilityTargetDataHandle(targetData);
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnOverlap.Broadcast(targetDataHandle);
        }
    }
}