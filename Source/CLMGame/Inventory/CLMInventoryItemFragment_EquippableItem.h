// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CLMInventoryItemDefinition.h"
#include "CLMInventoryItemFragment_EquippableItem.generated.h"

class UCLMInventoryItemDefinition;
/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMInventoryItemFragment_EquippableItem : public UCLMInventoryItemFragment
{
	GENERATED_BODY()
public:
	UCLMInventoryItemFragment_EquippableItem(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	UPROPERTY(EditAnywhere, Category = CLM)
	TSubclassOf<UCLMInventoryItemDefinition> EquipmentDefinition;
};
