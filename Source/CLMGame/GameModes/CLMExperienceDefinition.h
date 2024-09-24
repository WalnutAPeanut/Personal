// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CLMExperienceDefinition.generated.h"

class UCLMPawnData;
class UCLMExperienceActionSet;
class UGameFeatureAction;
/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UCLMExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> GameFeaturesToEnable;

	/// <summary>
	/// �ش� Property�� ��ŷ �� ����
	/// �ٸ� Plugin �۾��� ��� ���
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TObjectPtr<const UCLMPawnData> DefaultPawnData;

	/** ExperienceActionSet�� UGameFeatureAction�� Set�̸�, Gameplay �뵵�� �°� �з��� �������� ����Ѵ� */
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<TObjectPtr<UCLMExperienceActionSet>> ActionSets;

	/** �Ϲ����� GameFeatureAction���μ� �߰� */
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
