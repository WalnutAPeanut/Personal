// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CLMCameraMode.generated.h"

class UCLMCameraComponent;

struct FCLMCameraModeView
{
	FCLMCameraModeView();

	void Blend(const FCLMCameraModeView& Other, float OtherWeight);

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};

UENUM(BlueprintType)
enum class ECLMCameraModeBlendFunction : uint8
{
	Linear,
	/**
	 * EaseIn/Out은 exponent 값에 의해 조절된다:
	 */
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
};

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class CLMGAME_API UCLMCameraMode : public UObject
{
	GENERATED_BODY()
public:
	UCLMCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void UpdateCameraMode(float DeltaTime);
	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);

	UCLMCameraComponent* GetCLMCameraComponent() const;
	AActor* GetTargetActor() const;
	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;


public:
	/** CameraMode에 의해 생성된 CameraModeView */
	FCLMCameraModeView View;

	/** Camera Mode의 FOV */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", Clampmax = "170.0"))
	float FieldOfView;

	/** View에 대한 Pitch [Min, Max] */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMax;

	/** 얼마동안 Blend를 진행할지 시간을 의미 */
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	ECLMCameraModeBlendFunction BlendFunction;

	// EaseIn/Out에 사용한 Exponent
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent; 

	/** 선형적인 Blend 값 [0, 1] */
	float BlendAlpha;

	/**
	 * 해당 CameraMode의 최종 Blend 값
	 * 앞서 BlendAlpha의 선형 값을 매핑하여 최종 BlendWeight를 계산 (코드를 보며, 이해해보자)
	 */
	float BlendWeight;
};


/** Camera Blending을 담당하는 객체 */
UCLASS()
class UCLMCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UCLMCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UCLMCameraMode* GetCameraModeInstance(TSubclassOf<UCLMCameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<UCLMCameraMode>& CameraModeClass);
	void EvaluateStack(float DeltaTime, FCLMCameraModeView& OutCameraModeView);
	void UpdateStack(float DeltaTime);
	void BlendStack(FCLMCameraModeView& OutCameraModeView) const;

	 /** 생성된 CameraMode를 관리 */
	UPROPERTY()
	TArray<TObjectPtr<UCLMCameraMode>> CameraModeInstances;

	/** Camera Matrix Blend 업데이트 진행 큐 */
	UPROPERTY()
	TArray<TObjectPtr<UCLMCameraMode>> CameraModeStack;
};