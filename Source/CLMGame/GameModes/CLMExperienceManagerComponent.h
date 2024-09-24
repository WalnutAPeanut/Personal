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
 * 해당  Component는 Game state를 Owner로 가지면서 Experience의 상태 정보를 가지고 있는 Component이다.
 * Manager단어가 포함되어 있듯이 Experience 로딩 상태 업데이트 및 이벤트를 관리 한다.
 */
UCLASS()
class CLMGAME_API UCLMExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
	
public:

	bool IsExperienceLoaded() { return (LoadState == ECLMExperienceLoadState::Loaded) && (CurrentExperience != nullptr); }
	
	// 아래ㅔ의 OnExperienceLoaded에 바인딩 하거나, 이미 Experience 로딩이 완료 되었다면 바로 호출함
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

	// Experience의 로딩 상태를 모니터링
	ECLMExperienceLoadState LoadState = ECLMExperienceLoadState::Unloaded;
	// Experience 로딩이 완료된 이후, Broadcasting Delegate
	FOnCLMExperienceLoaded OnExperienceLoaded;

	/** 활성화된 GameFeature Plugin들 */
	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;
};
