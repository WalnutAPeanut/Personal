// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMInputConfig.h"
#include "CLMGame/CLMLogChannels.h"

UCLMInputConfig::UCLMInputConfig(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

const UInputAction* UCLMInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	// NativeInputActions�� ��ȸ�ϸ�, Input���� ���� InputTag�� �ִ��� üũ�Ѵ�:
	// - ������, �׿� ���� InputAction�� ��ȯ������, ���ٸ�, �׳� nullptr�� ��ȯ�Ѵ�.
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
