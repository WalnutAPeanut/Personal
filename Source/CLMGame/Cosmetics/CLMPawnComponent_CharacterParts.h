// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CLMCharacterPartTypes.h"
#include "CLMCosmeticAnimationTypes.h"
#include "Components/PawnComponent.h"
#include "CLMPawnComponent_CharacterParts.generated.h"

class UCLMPawnComponent_CharacterParts;

/** 인스턴스화 된 Character Part의 단위 */
USTRUCT()
struct FCLMAppliedCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part의 정의(메타 데이터) */
	UPROPERTY()
	FCLMCharacterPart Part;

	/** CLMCharacterPartList에서 할당 받은 Part 핸들값 (FCLMControllerCharacterPartEntry의 Handle 값과 같아야 함 -> 같으면 같은 Part) */
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	/** 인스턴스화 된 Character Part용 Actor */
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

/** CLMPawnComponent_CharacterParts에서 실질적 Character Parts를 관리하는 클래스 */
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

	/** 현재 인스턴스화된 Character Part */
	UPROPERTY()
	TArray<FCLMAppliedCharacterPartEntry> Entries;

	/** 해당 CLMCharacterPartList의 Owner인 PawnComponent */
	UPROPERTY()
	TObjectPtr<UCLMPawnComponent_CharacterParts> OwnerComponent;

	/** 앞서 보았던 PartHandle의 값을 할당 및 관리하는 변수 */
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

	/** 인스턴스화 된 Character Parts */
	UPROPERTY()
	FCLMCharacterPartList CharacterPartList;

	/** 애니메이션 적용을 위한 메시와 연결고리 */
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	FCLMAnimBodyStyleSelectionSet BodyMeshes;
};
