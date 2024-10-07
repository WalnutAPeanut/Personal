// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CLMEquipmentInstance.generated.h"

struct FCLMEquipmentActorToSpawn;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CLMGAME_API UCLMEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	UCLMEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//* Blueprint ���Ǹ� ���� Equip/Unequip �Լ�
	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = Equipment, meta = (DisplayName = "OnUnequipped"))
	void K2_OnUnequipped();

	UFUNCTION(BlueprintPure, Category = Equipment)
	TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

	UFUNCTION(BlueprintPure, Category = Equipment)
	UObject* GetInstigator() const { return Instigator; }

	UFUNCTION(BlueprintPure, Category = Equipment)
	APawn* GetPawn() const;

	void SpawnEquipmentActors(const TArray<FCLMEquipmentActorToSpawn>& ActorsToSpawn);
	void DestroyEquipmentActors();

	/**
	 * DeterminesOutputType�� C++ ���ǿ��� APawn* ��ȯ������, BP������ PawnType�� ���� OutputType�� �����ǵ��� �����̷�Ʈ(Redirect)�Ѵ�
	 */
	UFUNCTION(BlueprintPure, Category = Equipment, meta = (DeterminesOutputType = PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	/**
	* interfaces
	*/
	virtual void OnEquipped();
	virtual void OnUnequipped();

	/** � InventoryItemInstance�� ���� Ȱ��ȭ�Ǿ����� (����, QuickBarComponent���� ���� �ɰ��̴�) */
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	/** HakEquipementDefinition�� �°� Spawn�� Actor Instance�� */
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};
