// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CLMGameplayAbility.generated.h"

UENUM(BlueprintType)
enum class ECLMAbilityActivationPolicy : uint8
{
	/** Input이 Trigger 되었을 경우 (Presssed/Released) */
	OnInputTriggered,
	/** Input이 Held되어 있을 경우 */
	WhileInputActive,
	/** avatar가 생성되었을 경우, 바로 할당 */
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

	/** 언제 GA가 활성화될지 정책 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CLM|AbilityActivation")
	ECLMAbilityActivationPolicy ActivationPolicy;

	/** ability costs to apply CLMGameplayAbility separately */
	/*UPROPERTY(EditDefaultsOnly, Instanced, Category = "CLM|Costs")
	TArray<TObjectPtr<UCLMAbilityCost>> AdditionalCosts;*/
	
};
