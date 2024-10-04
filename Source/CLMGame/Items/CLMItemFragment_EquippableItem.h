// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CLMItemDefinition.h"
#include "CLMItemFragment_EquippableItem.generated.h"

class UCLMItemDefinition;
/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMItemFragment_EquippableItem : public UCLMItemFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = CLM)
	TSubclassOf<UCLMItemDefinition> EquipmentDefinition;
};
