// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "DDAbilityTask_WaitForCompOverlap.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGASWaitOverlapDelegate, const FGameplayAbilityTargetDataHandle&, TargetData);


UCLASS()
class DD_API UDDAbilityTask_WaitForCompOverlap : public UAbilityTask
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable)
    FGASWaitOverlapDelegate OnOverlap;

    virtual void Activate() override;

    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
    static UDDAbilityTask_WaitForCompOverlap* WaitForComponentOverlap(UGameplayAbility* OwningAbility, UPrimitiveComponent* Component);

private:
    virtual void OnDestroy(bool AbilityEnded) override;

    UFUNCTION()
    void OnHitCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY()
    UPrimitiveComponent* Component;

};
