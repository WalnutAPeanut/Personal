// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "../Input/CLMMappableConfigPair.h"
#include "CLMHeroComponent.generated.h"

class UCLMCameraMode;
class UCLMInputConfig;
struct FInputActionValue;

/**
 * 
 */
UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class CLMGAME_API UCLMHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	UCLMHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Overrides the camera from an active gameplay ability */
	//void SetAbilityCameraMode(TSubclassOf<UCLMCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle);

	/** Clears the camera override if it is set */
	//void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle);

	/** Adds mode-specific input config */
	//void AddAdditionalInputConfig(const UCLMInputConfig* InputConfig);

	///** Removes a mode-specific input config if it has been added */
	//void RemoveAdditionalInputConfig(const UCLMInputConfig* InputConfig);

	///** True if this is controlled by a real player and has progressed far enough in initialization where additional input bindings can be added */
	//bool IsReadyToBindInputs() const;

	/** Extension Event 이름 정의 */
	static const FName NAME_BindInputsNow;

	//~ Begin IGameFrameworkInitStateInterface interface
	static const FName NAME_ActorFeatureName;
	virtual FName GetFeatureName() const final { return NAME_ActorFeatureName; }
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;
	//~ End IGameFrameworkInitStateInterface interface


	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);

	TSubclassOf<UCLMCameraMode> DetermineCameraMode() const;

	UPROPERTY(EditAnywhere)
	TArray<FCLMMappableConfigPair> DefaultInputConfigs;
};
