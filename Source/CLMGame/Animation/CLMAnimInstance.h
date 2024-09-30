// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CLMAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	/** �ش� �Ӽ����� Lyra�� AnimBP���� ���Ǵ� ���̹Ƿ� ���������� */
	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;
	
};
