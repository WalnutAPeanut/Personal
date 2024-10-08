// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "CLMQuickBarComponent.generated.h"

class UCLMEquipmentManagerComponent;
class UCLMEquipmentInstance;
class UCLMItemInstance;

/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
	
public:
	UCLMQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, UCLMItemInstance* Item);

	UFUNCTION(BlueprintCallable, Category = "CLM")
	void SetActiveSlotIndex(int32 NewIndex);

protected:
	UCLMEquipmentManagerComponent* FindEquipmentManager() const;
	void UnequipItemInSlot();
	void EquipItemInSlot();

private:
	UPROPERTY()
	int32 NumSlots = 3;

	UPROPERTY()
	TArray<TObjectPtr<UCLMItemInstance>> Slots;

	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TObjectPtr<UCLMEquipmentInstance> EquippedItem;
};

