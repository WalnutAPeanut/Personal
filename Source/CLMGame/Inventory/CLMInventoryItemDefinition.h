// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CLMInventoryItemDefinition.generated.h"

class UCLMInventoryItemInstance;
/**
 * Inventory�� ���� Fragment�� Ȯ �ʹ��� ���� �� �ִ�:
 * - Lyra���� ����ϴ� ���ø� ���� �����غ���:
 *   - ULyraInventoryFragment_EquippableItem�� EquipmentItemDefinition�� ������ ������, ���� ������ �������� �ǹ��Ѵ�
 *   - ULyraInventoryFramgent_SetStats�� �����ۿ� ���� ������ ������ �ִ�
 *     - Rifle�� ���� SetStats���� �Ѿ�(Ammo)�� ���� ���� �ִ�ġ�� ���� ���� ��ź ���� ���÷� �� �� �ִ�
 *	 - ���...
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

	/** Inventory Item ����(��Ÿ) �̸� **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;

	/** Inventory Item�� Component�� Fragment�� �ν��ϸ� �ȴ� */
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = Display)
	TArray<TObjectPtr<UCLMInventoryItemFragment>> Fragments;
	
};
