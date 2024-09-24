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
	 * EaseIn/Out�� exponent ���� ���� �����ȴ�:
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
	/** CameraMode�� ���� ������ CameraModeView */
	FCLMCameraModeView View;

	/** Camera Mode�� FOV */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "5.0", UIMax = "170", ClampMin = "5.0", Clampmax = "170.0"))
	float FieldOfView;

	/** View�� ���� Pitch [Min, Max] */
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", Clampmax = "89.9"))
	float ViewPitchMax;

	/** �󸶵��� Blend�� �������� �ð��� �ǹ� */
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendTime;

	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	ECLMCameraModeBlendFunction BlendFunction;

	// EaseIn/Out�� ����� Exponent
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
	float BlendExponent; 

	/** �������� Blend �� [0, 1] */
	float BlendAlpha;

	/**
	 * �ش� CameraMode�� ���� Blend ��
	 * �ռ� BlendAlpha�� ���� ���� �����Ͽ� ���� BlendWeight�� ��� (�ڵ带 ����, �����غ���)
	 */
	float BlendWeight;
};


/** Camera Blending�� ����ϴ� ��ü */
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

	 /** ������ CameraMode�� ���� */
	UPROPERTY()
	TArray<TObjectPtr<UCLMCameraMode>> CameraModeInstances;

	/** Camera Matrix Blend ������Ʈ ���� ť */
	UPROPERTY()
	TArray<TObjectPtr<UCLMCameraMode>> CameraModeStack;
};