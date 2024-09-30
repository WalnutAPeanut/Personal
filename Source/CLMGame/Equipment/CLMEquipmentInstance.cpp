// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/CLMEquipmentInstance.h"

UCLMEquipmentInstance::UCLMEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

APawn* UCLMEquipmentInstance::GetPawn() const
{
	return nullptr;
}

void UCLMEquipmentInstance::SpawnEquipmentActors(const TArray<FCLMEquipmentActorToSpawn>& ActorsToSpawn)
{
}

void UCLMEquipmentInstance::DestroyEquipmentActors()
{
}

void UCLMEquipmentInstance::OnEquipped()
{
}

void UCLMEquipmentInstance::OnUnequipped()
{
}
