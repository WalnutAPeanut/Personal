// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CLMEquipmentDefinition.generated.h"

class UCLMEquipmentInstance;
//class UCLMAbilitySet;

USTRUCT()
struct FCLMEquipmentActorToSpawn
{
	GENERATED_BODY()

	/** Spawn�� ��� Actor Ŭ���� (== Actor�� ��ӹ��� Asset���� �����ص� ��) */
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	/** ��� Bone Socket�� ������ �����Ѵ� */
	UPROPERTY(EditAnywhere, Category = Equipment)
	FName AttachSocket;

	/** Socket���� ������� Transformation�� ���Ұ����� ����: (Rotation, Position, Scale) */
	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform AttachTransform;
};

/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMEquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	UCLMEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// * �ش� ��Ÿ �����͸� ����ϸ�, � �ν��Ͻ��� Spawn���� �����ϴ� Ŭ���� 
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TSubclassOf<UCLMEquipmentInstance> InstanceType;

	// * �ش� ���� �������� ����ϸ�, � Actor�� Spawn�� �Ǵ��� ������ ��� �ִ�
	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FCLMEquipmentActorToSpawn> ActorsToSpawn;

	//// * ������ ���� �ο� ������ Ability Set
	//UPROPERTY(EditDefaultsOnly, Category = Equipment)
	//TArray<TObjectPtr<UCLMAbilitySet>> AbilitySetsToGrant;
};
