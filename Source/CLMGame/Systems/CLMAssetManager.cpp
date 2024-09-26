// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMAssetManager.h"
#include "Misc/App.h"
#include "Stats/StatsMisc.h"
#include "Engine/Engine.h"
#include "Misc/ScopedSlowTask.h"
#include "CLMGame/CLMGameplayTags.h"
#include "CLMGame/CLMLogChannels.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(CLMAssetManager)

UCLMAssetManager::UCLMAssetManager()
{
}

UCLMAssetManager& UCLMAssetManager::Get()
{
	check(GEngine);

	if (UCLMAssetManager* Singleton = Cast<UCLMAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogCLM, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to CLMAssetManager!"));
	// Fatal error above prevents this from being called.
	return *NewObject<UCLMAssetManager>();
}

void UCLMAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCLMGameplayTags::InitializeNativeTags();
}


bool UCLMAssetManager::ShouldLogAssetLoads()
{
	const TCHAR* CommandLineContent = FCommandLine::Get();
	static bool bLogAssetLoads = FParse::Param(CommandLineContent, TEXT("LogAssetLoads"));
	return bLogAssetLoads;
}

/// <summary>
/// 현재 필요한 데이터만 Load 하는 함수
/// </summary>
/// <param name="AssetPath"></param>
/// <returns></returns>
UObject* UCLMAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	if (AssetPath.IsValid())
	{
		TUniquePtr<FScopeLogTime> LogTimePtr;

		if (ShouldLogAssetLoads())
		{
			//단순히 로깅하면서 초단위로 로깅 진행
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// 1. AssetManager가 있으면, AssetMaanger의 StreamableMaanger를 통해 정적 로딩
		// 2. 아니면 FSoftObjectPath를 통해 바로 정적 로딩
		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// Use LoadObject if asset manager isn't ready yet.
		// StaticFindObject를 통해 확인하고 실패하면 진짜 로딩
		// 해당위로 로딩시 속도가 매우 느림 
		return AssetPath.TryLoad();
	}

	return nullptr;
}


void UCLMAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock LoadedAssetsLock(&LoadedAssetsCritical);
		LoadedAssets.Add(Asset);
	}
}
