// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "CLMEquipmentManagerComponent.generated.h"

class UCLMEquipmentDefinition;
class UCLMEquipmentInstance;

USTRUCT(BlueprintType)
struct FCLMAppliedEquipmentEntry
{
	GENERATED_BODY()
	
	// 장착물에 대한 메터 데이터
	UPROPERTY()
	TSubclassOf<UCLMEquipmentDefinition> EquipmentDefinition;

	//EquipmentDefinition을 통해 생선된 인스턴스
	UPROPERTY()
	TObjectPtr<UCLMEquipmentInstance> Instance = nullptr;

};

USTRUCT(BlueprintType)
struct FCLMEquipmentList
{
	GENERATED_BODY()

	FCLMEquipmentList(UActorComponent* InOwnerComponent = nullptr) : OwnerComponent(InOwnerComponent)
	{}

	UCLMEquipmentInstance* AddEntry(TSubclassOf<UCLMEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UCLMEquipmentInstance* Instance);

	// 장착물에 대한 관리 리스트
	UPROPERTY()
	TArray<FCLMAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:
	UCLMEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UCLMEquipmentInstance* EquipItem(TSubclassOf<UCLMEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(UCLMEquipmentInstance* ItemInstance);

	UCLMEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UCLMEquipmentInstance> InstanceType);

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

	UFUNCTION(BlueprintCallable)
	TArray<UCLMEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UCLMEquipmentInstance> InstanceType) const;

	UPROPERTY()
	FCLMEquipmentList EquipmentList;
};
