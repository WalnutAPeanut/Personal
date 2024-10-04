// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMEquipmentDefinition.h"
#include "CLMEquipmentInstance.h"

UCLMEquipmentDefinition::UCLMEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	InstanceType = UCLMEquipmentInstance::StaticClass();
}
