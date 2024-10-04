// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMItemInstance.h"
#include "CLMItemDefinition.h"

UCLMItemInstance::UCLMItemInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

const UCLMItemFragment* UCLMItemInstance::FindFragmentByClass(TSubclassOf<UCLMItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		// EditDefaultsOnly로 선언되어 있으므로, GetDefault로 가져와도 무관하다
		// - Fragment 정보는 Instance가 아닌 Definition에 있다
		return GetDefault<UCLMItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}
