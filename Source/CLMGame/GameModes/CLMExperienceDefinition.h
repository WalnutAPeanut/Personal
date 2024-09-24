// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLMExperienceDefinition.generated.h"

class UCLMPawnData;
class UCLMExperienceActionSet;
class UGameFeatureAction;
/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UCLMExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	/// <summary>
	/// 해당 Property는 마킹 및 기억용
	/// 다른 Plugin 작업할 경우 사용
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<const UCLMPawnData> DefaultPawnData;

	/** ExperienceActionSet은 UGameFeatureAction의 Set이며, Gameplay 용도에 맞게 분류의 목적으로 사용한다 */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<TObjectPtr<UCLMExperienceActionSet>> ActionSets;

	/** 일반적인 GameFeatureAction으로서 추가 */
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
