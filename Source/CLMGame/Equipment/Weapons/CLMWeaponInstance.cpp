// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/Weapons/CLMWeaponInstance.h"

UCLMWeaponInstance::UCLMWeaponInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UCLMWeaponInstance::OnEquipped()
{
	Super::OnEquipped();
	UWorld* World = GetWorld();
	check(World);
	/*TimeLastEquipped = World->GetTimeSeconds();
	ApplyDeviceProperties();*/
}

void UCLMWeaponInstance::OnUnequipped()
{
	Super::OnUnequipped();

	//RemoveDeviceProperties();
}

TSubclassOf<UAnimInstance> UCLMWeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
	const FCLMAnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnequippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}
