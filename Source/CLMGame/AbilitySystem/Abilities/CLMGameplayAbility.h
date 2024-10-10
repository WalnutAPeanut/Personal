// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CLMGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ECLMAbilityActivationPolicy : uint8
{
	/** Input�� Trigger �Ǿ��� ��� (Presssed/Released) */
	OnInputTriggered,
	/** Input�� Held�Ǿ� ���� ��� */
	WhileInputActive,
	/** avatar�� �����Ǿ��� ���, �ٷ� �Ҵ� */
	OnSpawn,
};

/** forward declarations */
class UCLMAbilityCost;

/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UCLMGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** ���� GA�� Ȱ��ȭ���� ��å */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CLM|AbilityActivation")
	ECLMAbilityActivationPolicy ActivationPolicy;

	/** ability costs to apply CLMGameplayAbility separately */
	/*UPROPERTY(EditDefaultsOnly, Instanced, Category = "CLM|Costs")
	TArray<TObjectPtr<UCLMAbilityCost>> AdditionalCosts;*/
	
};
