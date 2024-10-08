// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CLMItemManagerComponent.generated.h"

class UCLMItemInstance;
class UCLMItemDefinition;

USTRUCT(BlueprintType)
struct FCLMItemEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UCLMItemInstance> Instance = nullptr;
};

USTRUCT(BlueprintType)
struct FCLMItemList
{
	GENERATED_BODY()

	FCLMItemList(UActorComponent* InOwnerComponent = nullptr) : OwnerComponent(InOwnerComponent)
	{}

	UCLMItemInstance* AddEntry(TSubclassOf<UCLMItemDefinition> ItemDef);

	UPROPERTY()
	TArray<FCLMItemEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLMGAME_API UCLMItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCLMItemManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** InventoryItemDefinition을 통해, InventoryList에 추가하여 관리하며, InventoryItemInstance를 반환한다 */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	UCLMItemInstance* AddItemDefinition(TSubclassOf<UCLMItemDefinition> ItemDef);

	UPROPERTY()
	FCLMItemList ItemList;
	 
	virtual void BeginPlay() override;
};
