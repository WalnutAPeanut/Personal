// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMItemDefinition.h"

UCLMItemDefinition::UCLMItemDefinition(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

const UCLMItemFragment* UCLMItemDefinition::FindFragmentByClass(TSubclassOf<UCLMItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		// Fragments�� ��ȸ�ϸ�, IsA()�� ���� �ش� Ŭ������ ������ �ִ��� Ȯ���Ѵ�:
		for (UCLMItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}
