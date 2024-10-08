// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMQuickBarComponent.h"
#include "CLMEquipmentManagerComponent.h"
#include "CLMEquipmentInstance.h"
#include "Items/CLMItemInstance.h"
#include "Items/CLMItemFragment_EquippableItem.h"

UCLMQuickBarComponent::UCLMQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UCLMQuickBarComponent::BeginPlay()
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}

	Super::BeginPlay();
}

UCLMEquipmentManagerComponent* UCLMQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<UCLMEquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void UCLMQuickBarComponent::UnequipItemInSlot()
{
	if (UCLMEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem)
		{
			EquipmentManager->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}

void UCLMQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	if (UCLMItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		if (const UCLMItemFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UCLMItemFragment_EquippableItem>())
		{
			if(TSubclassOf<UCLMEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition)
			{
				if (UCLMEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem)
					{
						EquippedItem->SetInstigator(SlotItem);
					}
				}
			}
		}
	}
}

void UCLMQuickBarComponent::AddItemToSlot(int32 SlotIndex, UCLMItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
		}
	}
}

void UCLMQuickBarComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
	}
}


