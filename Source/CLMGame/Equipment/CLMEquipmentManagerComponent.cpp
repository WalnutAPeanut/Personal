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
	// �ܼ��� �׳� Entries�� ��ȸ�ϸ�, Instance�� ã�Ƽ�
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FCLMAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			// TakeFromAbilitySystem�� GiveToAbilitySystem �ݴ� ��Ȱ��, ActivatableAbilities���� �����Ѵ�
			UCLMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			check(ASC);
			Entry.GrantedHandles.TakeFromAbilitySystem(ASC);

			// Actor ���� �۾� �� iterator�� ���� �����ϰ� Array���� ���� ����
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
		// �ش� �Լ��� BP�� Event��带 ȣ�����ش� (�ڼ��Ѱ� �ش� �Լ� �����ϸ鼭 ����)
		ItemInstance->OnUnequipped();

		// EquipementList�� �������ش�:
		// - �����ϴ� ������ ���� �߰��Ǿ��� Actor Instance�� ���Ÿ� �����Ѵ�
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

	// EquipmentList�� ��ȸ�ϸ�
	for (const FCLMAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UCLMEquipmentInstance* Instance = Entry.Instance)
		{
			// InstanceType�� �´� Class�̸� Results�� �߰��Ͽ� ��ȯ
			// - �츮�� ���, HakRangedWeaponInstance�� �ɰ���
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}
