// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "CLMPawnExtensionComponent.generated.h"

class UCLMPawnData;
class UObject;

/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:
	UCLMPawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ Begin IGameFrameworkInitStateInterface interface
	static const FName NAME_ActorFeatureName;
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;
	//~ End IGameFrameworkInitStateInterface interface

	UFUNCTION(BlueprintPure, Category = "CLM|Pawn")
	static UCLMPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCLMPawnExtensionComponent>() : nullptr); }

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UCLMPawnData* InPawnData);

	void SetupPlayerInputComponent();

	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	UPROPERTY(EditInstanceOnly, Category = "CLM|Pawn")
	TObjectPtr<const UCLMPawnData> PawnData;
};
