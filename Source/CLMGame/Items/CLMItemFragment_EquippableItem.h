// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CLMItemDefinition.h"
#include "Equipment/CLMEquipmentDefinition.h"
#include "CLMItemFragment_EquippableItem.generated.h"

class UCLMEquipmentDefinition;
/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMItemFragment_EquippableItem : public UCLMItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = CLM)
	TSubclassOf<UCLMEquipmentDefinition> EquipmentDefinition;
};
