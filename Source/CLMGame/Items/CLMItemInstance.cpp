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
		// EditDefaultsOnly�� ����Ǿ� �����Ƿ�, GetDefault�� �����͵� �����ϴ�
		// - Fragment ������ Instance�� �ƴ� Definition�� �ִ�
		return GetDefault<UCLMItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}
