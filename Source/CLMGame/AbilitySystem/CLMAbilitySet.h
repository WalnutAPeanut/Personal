// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpec.h"
#include "CLMAbilitySet.generated.h"

class UCLMAbilitySystemComponent;
class UCLMGameplayAbility;

USTRUCT(BlueprintType)
struct FCLMAbilitySet_GameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCLMGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct FCLMAbilitySet_GrantedHandles
{
	GENERATED_BODY()

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void TakeFromAbilitySystem(UCLMAbilitySystemComponent* CLMASC);
protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};
/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UCLMAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(UCLMAbilitySystemComponent* ASC, FCLMAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities")
	TArray<FCLMAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};
