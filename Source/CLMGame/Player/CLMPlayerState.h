// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "ModularPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "CLMPlayerState.generated.h"

class UCLMPawnData;
class UCLMExperienceDefinition;
class UCLMAbilitySystemComponent;
class UAbilitySystemComponent;
class ACLMPlayerController;

/**
 * 
 */
UCLASS(Config = Game)
class CLMGAME_API ACLMPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ACLMPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "CLM|PlayerState")
	ACLMPlayerController* GetCLMPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "CLM|PlayerState")
	UCLMAbilitySystemComponent* GetCLMAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	void SetPawnData(const UCLMPawnData* InPawnData);
	
	//~AActor interface
	//virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() final;
	//~End of AActor interface

private:
	void OnExperienceLoaded(const UCLMExperienceDefinition* CurrentExperience);

protected:
	UPROPERTY()
	TObjectPtr<const UCLMPawnData> PawnData;

	UPROPERTY()
	TObjectPtr<UCLMAbilitySystemComponent> AbilitySystemComponent;
};
