// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CLMItemDefinition.generated.h"

class UCLMItemInstance;

UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class UCLMItemFragment : public UObject
{
	GENERATED_BODY()
public:
	virtual void OnInstanceCreated(UCLMItemInstance* Instance) const {}
};

/**
 * 
 */
UCLASS(Blueprintable)
class CLMGAME_API UCLMItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	UCLMItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UCLMItemFragment* FindFragmentByClass(TSubclassOf<UCLMItemFragment> FragmentClass) const;

	/**  Item ����(��Ÿ) �̸� **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;

	/**  Item�� Component�� Fragment�� �ν��ϸ� �ȴ� */
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = Display)
	TArray<TObjectPtr<UCLMItemFragment>> Fragments;
};
