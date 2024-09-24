// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CLMPlayerState.generated.h"

class UCLMPawnData;
class UCLMExperienceDefinition;

/**
 * 
 */
UCLASS()
class CLMGAME_API ACLMPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ACLMPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() final;

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void OnExperienceLoaded(const UCLMExperienceDefinition* CurrentExperience);
	void SetPawnData(const UCLMPawnData* InPawnData);

	UPROPERTY()
	TObjectPtr<const UCLMPawnData> PawnData;
};
