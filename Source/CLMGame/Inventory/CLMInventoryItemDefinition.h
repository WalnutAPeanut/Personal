// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CLMInventoryItemDefinition.generated.h"

class UCLMInventoryItemInstance;
/**
 * Inventory에 대한 Fragment은 확 와닫지 않을 수 있다:
 * - Lyra에서 사용하는 예시를 통해 이해해보자:
 *   - ULyraInventoryFragment_EquippableItem은 EquipmentItemDefinition을 가지고 있으며, 장착 가능한 아이템을 의미한다
 *   - ULyraInventoryFramgent_SetStats는 아이템에 대한 정보를 가지고 있다
 *     - Rifle에 대한 SetStats으로 총알(Ammo)에 대한 장착 최대치와 현재 남은 잔탄 수를 예시로 들 수 있다
 *	 - 등등...
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class UCLMInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:
	/** interface to call when inventory item instance is added to UCLMInventoryManagerComponent's InventoryList */
	virtual void OnInstanceCreated(UCLMInventoryItemInstance* Instance) const {}
};

/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	UCLMInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UCLMInventoryItemFragment* FindFragmentByClass(TSubclassOf<UCLMInventoryItemFragment> FragmentClass) const;

	/** Inventory Item 정의(메타) 이름 **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;

	/** Inventory Item의 Component를 Fragment로 인식하면 된다 */
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = Display)
	TArray<TObjectPtr<UCLMInventoryItemFragment>> Fragments;
	
};
