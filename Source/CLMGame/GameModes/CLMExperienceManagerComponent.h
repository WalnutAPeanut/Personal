// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
#include "CLMExperienceManagerComponent.generated.h"

class UCLMExperienceDefinition;

enum class ECLMExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	ExecutingActions,
	Loaded,
	Deactivating,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCLMExperienceLoaded, const UCLMExperienceDefinition*);
/**
 * �ش�  Component�� Game state�� Owner�� �����鼭 Experience�� ���� ������ ������ �ִ� Component�̴�.
 * Manager�ܾ ���ԵǾ� �ֵ��� Experience �ε� ���� ������Ʈ �� �̺�Ʈ�� ���� �Ѵ�.
 */
UCLASS()
class CLMGAME_API UCLMExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:

	bool IsExperienceLoaded() { return (LoadState == ECLMExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }
	
	// �Ʒ����� OnExperienceLoaded�� ���ε� �ϰų�, �̹� Experience �ε��� �Ϸ� �Ǿ��ٸ� �ٷ� ȣ����
	void CallOrRegister_OnExperienceLoaded(FOnCLMExperienceLoaded::FDelegate&& Delegate);

	void ServerSetCurrentExperience(FPrimaryAssetId ExperienceId);
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();

	const UCLMExperienceDefinition* GetCurrentExperienceChecked() const;

public:
	UPROPERTY()
	TObjectPtr<const UCLMExperienceDefinition> CurrentExperience;

	// Experience�� �ε� ���¸� ����͸�
	ECLMExperienceLoadState LoadState = ECLMExperienceLoadState::Unloaded;
	// Experience �ε��� �Ϸ�� ����, Broadcasting Delegate
	FOnCLMExperienceLoaded OnExperienceLoaded;

	/** Ȱ��ȭ�� GameFeature Plugin�� */
	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
};
