// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CLMGameModeBase.generated.h"

class UCLMExperienceDefinition;
class UCLMPawnData;
/**
 * 
 */
UCLASS()
class CLMGAME_API ACLMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACLMGameModeBase();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	void InitGameState() final;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) final;
	// HandleStartingNewPlayer
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;
	// SpawnDefaultPawnAtTransform
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;

	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	bool IsExperienceLoaded();
	void OnExperienceLoaded(const UCLMExperienceDefinition* CurrentExperience);

	const UCLMPawnData* GetPawnDataForController(const AController* InController) const;
};
