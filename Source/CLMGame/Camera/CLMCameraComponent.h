// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "CLMCameraMode.h"
#include "CLMCameraComponent.generated.h"

/** template forward declaration */
template <class TClass> class TSubclassOf;

/** (return type, delegate_name) */
DECLARE_DELEGATE_RetVal(TSubclassOf<UCLMCameraMode>, FCLMCameraModeDelegate);
/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
	
public:
	UCLMCameraComponent(const FObjectInitializer& ObjectInitializer);

	static UCLMCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCLMCameraComponent>() : nullptr); }

	virtual void OnRegister() final;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

	AActor* GetTargetActor() const { return GetOwner(); }

	void UpdateCameraModes();

	 /** ī�޶��� blending ����� �����ϴ� stack */
	UPROPERTY()
	TObjectPtr<UCLMCameraModeStack> CameraModeStack;

	/** ���� CameraMode�� �������� Delegate */
	FCLMCameraModeDelegate DetermineCameraModeDelegate;
};
