// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLMUserFacingExperience.generated.h"

class UCommonSession_HostSessionRequest;
/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMUserFacingExperience : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	UCommonSession_HostSessionRequest* CreateHostingRequest() const;

	/** The specific map to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	/** The gameplay experience to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Experience, meta = (AllowedTypes = "CLMExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
	
};
