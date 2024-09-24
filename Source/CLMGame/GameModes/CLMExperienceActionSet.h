// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLMExperienceActionSet.generated.h"

class UGameFeatureAction;
/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UCLMExperienceActionSet();

	UPROPERTY(EditAnywhere, Category = "Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
	
};
