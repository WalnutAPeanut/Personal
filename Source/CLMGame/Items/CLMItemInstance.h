// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "../CLMGameplayTags.h"
#include "CLMItemInstance.generated.h"

class UCLMItemFragment;
class UCLMItemDefinition;
/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UCLMItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = FragmentClass))
	const UCLMItemFragment* FindFragmentByClass(TSubclassOf<UCLMItemFragment> FragmentClass) const;
	
	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	///** add/remove stack count to stat tag(=gameplay-tag stack) */
	//void AddStatTagStack(FGameplayTag Tag, int32 StackCount);
	//void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	///** whether stat tag has in StatTags */
	//bool HasStatTag(FGameplayTag Tag) const;

	///** get the current count of gameplay-tag stack */
	//UFUNCTION(BlueprintCallable, Category = Inventory)
	//int32 GetStatTagStackCount(FGameplayTag Tag) const;

	///** gameplay-tag stacks for inventory item instance */
	//UPROPERTY()
	//FHakGameplayTagStackContainer StatTags;

	UPROPERTY()
	TSubclassOf<UCLMItemDefinition> ItemDef;
};
