// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMEquipmentManagerComponent.h"
#include "CLMEquipmentDefinition.h"
#include "CLMEquipmentInstance.h"
#include "CLMGame/AbilitySystem/CLMAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UCLMEquipmentInstance* FCLMEquipmentList::AddEntry(TSubclassOf<UCLMEquipmentDefinition> EquipmentDefinition)
{
	UCLMEquipmentInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	const UCLMEquipmentDefinition* EquipmentCDO = GetDefault<UCLMEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UCLMEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UCLMEquipmentInstance::StaticClass();
	}

	FCLMAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UCLMEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;

	UCLMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);
	for (const TObjectPtr<UCLMAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
	{
		AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);
	return Result;
}

void FCLMEquipmentList::RemoveEntry(UCLMEquipmentInstance* Instance)
{
	// 단순히 그냥 Entries를 순회하며, Instance를 찾아서
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FCLMAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			// TakeFromAbilitySystem은 GiveToAbilitySystem 반대 역활로, ActivatableAbilities에서 제거한다
			UCLMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			check(ASC);
			Entry.GrantedHandles.TakeFromAbilitySystem(ASC);

			// Actor 제거 작업 및 iterator를 통한 안전하게 Array에서 제거 진행
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
}

UCLMAbilitySystemComponent* FCLMEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();

	return Cast<UCLMAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UCLMEquipmentManagerComponent::UCLMEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	,EquipmentList(this)
{
}

UCLMEquipmentInstance* UCLMEquipmentManagerComponent::EquipItem(TSubclassOf<UCLMEquipmentDefinition> EquipmentDefinition)
{
	UCLMEquipmentInstance* Result = nullptr;
	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();
		}
	}
	return Result;
}

void UCLMEquipmentManagerComponent::UnequipItem(UCLMEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		// 해당 함수는 BP의 Event노드를 호출해준다 (자세한건 해당 함수 구현하면서 보자)
		ItemInstance->OnUnequipped();

		// EquipementList에 제거해준다:
		// - 제거하는 과정을 통해 추가되었던 Actor Instance를 제거를 진행한다
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

UCLMEquipmentInstance* UCLMEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UCLMEquipmentInstance> InstanceType)
{
	for (FCLMAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UCLMEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}
	return nullptr;
}

TArray<UCLMEquipmentInstance*> UCLMEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UCLMEquipmentInstance> InstanceType) const
{
	TArray<UCLMEquipmentInstance*> Results;

	// EquipmentList를 순회하며
	for (const FCLMAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UCLMEquipmentInstance* Instance = Entry.Instance)
		{
			// InstanceType에 맞는 Class이면 Results에 추가하여 반환
			// - 우리의 경우, HakRangedWeaponInstance가 될거임
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}
