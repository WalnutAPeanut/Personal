// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CLMCharacterPartTypes.h"
#include "CLMCosmeticAnimationTypes.h"
#include "Components/PawnComponent.h"
#include "CLMPawnComponent_CharacterParts.generated.h"

class UCLMPawnComponent_CharacterParts;

/** �ν��Ͻ�ȭ �� Character Part�� ���� */
USTRUCT()
struct FCLMAppliedCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part�� ����(��Ÿ ������) */
	UPROPERTY()
	FCLMCharacterPart Part;

	/** CLMCharacterPartList���� �Ҵ� ���� Part �ڵ鰪 (FCLMControllerCharacterPartEntry�� Handle ���� ���ƾ� �� -> ������ ���� Part) */
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	/** �ν��Ͻ�ȭ �� Character Part�� Actor */
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

/** CLMPawnComponent_CharacterParts���� ������ Character Parts�� �����ϴ� Ŭ���� */
USTRUCT(BlueprintType)
struct FCLMCharacterPartList
{
	GENERATED_BODY()

	FCLMCharacterPartList()
		: OwnerComponent(nullptr)
	{}

	FCLMCharacterPartList(UCLMPawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{}

	bool SpawnActorForEntry(FCLMAppliedCharacterPartEntry& Entry);
	void DestroyActorForEntry(FCLMAppliedCharacterPartEntry& Entry);

	FCLMCharacterPartHandle AddEntry(FCLMCharacterPart NewPart);
	void RemoveEntry(FCLMCharacterPartHandle Handle);

	FGameplayTagContainer CollectCombinedTags() const;

	/** ���� �ν��Ͻ�ȭ�� Character Part */
	UPROPERTY()
	TArray<FCLMAppliedCharacterPartEntry> Entries;

	/** �ش� CLMCharacterPartList�� Owner�� PawnComponent */
	UPROPERTY()
	TObjectPtr<UCLMPawnComponent_CharacterParts> OwnerComponent;

	/** �ռ� ���Ҵ� PartHandle�� ���� �Ҵ� �� �����ϴ� ���� */
	int32 PartHandleCounter = 0;
};

/**
 *
 */
UCLASS()
class CLMGAME_API UCLMPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
public:
	UCLMPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Cosmetics)
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;
	void BroadcastChanged();

	FCLMCharacterPartHandle AddCharacterPart(const FCLMCharacterPart& NewPart);
	void RemoveCharacterPart(FCLMCharacterPartHandle Handle);

	/** �ν��Ͻ�ȭ �� Character Parts */
	UPROPERTY()
	FCLMCharacterPartList CharacterPartList;

	/** �ִϸ��̼� ������ ���� �޽ÿ� ����� */
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FCLMAnimBodyStyleSelectionSet BodyMeshes;
};
