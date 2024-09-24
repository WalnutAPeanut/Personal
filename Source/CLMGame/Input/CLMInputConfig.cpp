// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMInputConfig.h"
#include "CLMGame/CLMLogChannels.h"

UCLMInputConfig::UCLMInputConfig(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

const UInputAction* UCLMInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	// NativeInputActions을 순회하며, Input으로 들어온 InputTag가 있는지 체크한다:
	// - 있으면, 그에 따른 InputAction을 반환하지만, 없다면, 그냥 nullptr을 반환한다.
	for (const FCLMInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogCLM, Error, TEXT("can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* UCLMInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FCLMInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogCLM, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
