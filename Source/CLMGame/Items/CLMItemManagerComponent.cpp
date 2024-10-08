// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMItemManagerComponent.h"
#include "CLMItemDefinition.h"
#include "CLMItemInstance.h"

UCLMItemInstance* FCLMItemList::AddEntry(TSubclassOf<UCLMItemDefinition> ItemDef)
{
	UCLMItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FCLMItemEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UCLMItemInstance>(OwningActor);
	NewEntry.Instance->ItemDef = ItemDef;

	// iterating fragments and call callback to OnInstanceCreated()
	for (const UCLMItemFragment* Fragment : GetDefault<UCLMItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}

	Result = NewEntry.Instance;
	return Result;
}

UCLMItemManagerComponent::UCLMItemManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	,ItemList(this)
{
}

UCLMItemInstance* UCLMItemManagerComponent::AddItemDefinition(TSubclassOf<UCLMItemDefinition> ItemDef)
{
	UCLMItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = ItemList.AddEntry(ItemDef);
	}
	return Result;
}

void UCLMItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	int a = 10;
	int b = 20;
	int c = 30;
}
