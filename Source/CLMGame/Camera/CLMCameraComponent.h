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

	 /** 카메라의 blending 기능을 지원하는 stack */
	UPROPERTY()
	TObjectPtr<UCLMCameraModeStack> CameraModeStack;

	/** 현재 CameraMode를 가져오는 Delegate */
	FCLMCameraModeDelegate DetermineCameraModeDelegate;
};
