// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularPlayerController.h"
#include "CLMPlayerController.generated.h"

class UCLMAbilitySystemComponent;
class ACLMPlayerState;
/**
 * 
 */
UCLASS()
class CLMGAME_API ACLMPlayerController : public AModularPlayerController
{
	GENERATED_BODY()

public:
	ACLMPlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "CLM|PlayerController")
	ACLMPlayerState* GetCLMPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "CLM|PlayerController")
	UCLMAbilitySystemComponent* GetCLMAbilitySystemComponent() const;

	//~AController interface
	virtual void OnPossess(APawn* aPawn) override;
	//~End of AController interface

	//~APlayerController interface
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController interface

};
