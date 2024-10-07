// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/Weapons/CLMWeaponInstance.h"

UCLMWeaponInstance::UCLMWeaponInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

TSubclassOf<UAnimInstance> UCLMWeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
	const FCLMAnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnequippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}
