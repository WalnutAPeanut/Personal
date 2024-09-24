// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CLMGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	/**
	 * UGameInstance's interfaces
	 */
	virtual void Init() override;
	virtual void Shutdown() override;
};
