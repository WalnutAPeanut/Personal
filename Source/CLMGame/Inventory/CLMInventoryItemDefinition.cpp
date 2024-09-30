// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMInventoryItemDefinition.h"

UCLMInventoryItemDefinition::UCLMInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

const UCLMInventoryItemFragment* UCLMInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UCLMInventoryItemFragment> FragmentClass) const
{
	return nullptr;
}
