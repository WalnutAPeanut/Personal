// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLMPawnData.generated.h"

class UCLMCameraMode;
class UCLMInputConfig;
/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCLMPawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CLM|Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CLM|Camera")
	TSubclassOf<UCLMCameraMode> DefaultCameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CLM|InputConfig")
	TObjectPtr<UCLMInputConfig> InputConfig;
};
