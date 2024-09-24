// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CLMCameraMode.h"
#include "UCLMCameraMode_ThirdPerson.generated.h"

class UCurveVector;
/**
 * 
 */
// �������Ʈ���� ����� �޾Ƽ��� ����� ������ �ɼ� ����
UCLASS(Abstract, Blueprintable)
class CLMGAME_API UUCLMCameraMode_ThirdPerson : public UCLMCameraMode
{
	GENERATED_BODY()
public:
	UUCLMCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void UpdateView(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Third Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve;
};
