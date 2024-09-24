// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CLMGameState.generated.h"

class UCLMExperienceManagerComponent;
class UCLMPawnData;
/**
 * 
 */
UCLASS()
class CLMGAME_API ACLMGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ACLMGameState();

public:
	UPROPERTY()
	TObjectPtr<UCLMExperienceManagerComponent> ExperienceManagerComponent;
};
