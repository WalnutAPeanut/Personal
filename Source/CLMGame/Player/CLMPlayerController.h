// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularPlayerController.h"
#include "CLMPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CLMGAME_API ACLMPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ACLMPlayerController(const FObjectInitializer& ObjectInitializer);


	virtual void OnPossess(APawn* aPawn) override;
};
