// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CLMAbilitySet.h"
#include "AbilitySystem/CLMAbilitySystemComponent.h"
#include "Abilities/CLMGameplayAbility.h"

void FCLMAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FCLMAbilitySet_GrantedHandles::TakeFromAbilitySystem(UCLMAbilitySystemComponent* CLMASC)
{
	// 어떠한 기능일까?
	if (CLMASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			CLMASC->ClearAbility(Handle);
		}
	}
}

UCLMAbilitySet::UCLMAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCLMAbilitySet::GiveToAbilitySystem(UCLMAbilitySystemComponent* ASC, FCLMAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	check(ASC);
	if (!ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FCLMAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		if (!IsValid(AbilityToGrant.Ability))
		{
			continue;
		}

		// 현재는 CDO 로 가져오지만 능력이 모두 동일하지 않을경우에는 데이터를 추가해야한다.
		UCLMGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UCLMGameplayAbility>();
		
		FGameplayAbilitySpec AbilitSpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitSpec.SourceObject = SourceObject;
		AbilitSpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitSpecHandle = ASC->GiveAbility(AbilitSpec);
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitSpecHandle);
		}
	}
}
