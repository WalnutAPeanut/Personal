// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/CLMEquipmentInstance.h"
#include "Cosmetics/CLMCosmeticAnimationTypes.h"
#include "CLMWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMWeaponInstance : public UCLMEquipmentInstance
{
	GENERATED_BODY()
public:
	UCLMWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FCLMAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FCLMAnimLayerSelectionSet UnequippedAnimSet;
};
