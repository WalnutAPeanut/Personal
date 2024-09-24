// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CLMMappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

USTRUCT()
struct FCLMMappableConfigPair
{
	GENERATED_BODY()

	FCLMMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};
