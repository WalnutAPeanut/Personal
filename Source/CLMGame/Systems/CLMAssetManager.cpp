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
/// ���� �ʿ��� �����͸� Load �ϴ� �Լ�
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
			//�ܼ��� �α��ϸ鼭 �ʴ����� �α� ����
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("Synchronously loaded asset [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// 1. AssetManager�� ������, AssetMaanger�� StreamableMaanger�� ���� ���� �ε�
		// 2. �ƴϸ� FSoftObjectPath�� ���� �ٷ� ���� �ε�
		if (UAssetManager::IsInitialized())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}

		// Use LoadObject if asset manager isn't ready yet.
		// StaticFindObject�� ���� Ȯ���ϰ� �����ϸ� ��¥ �ε�
		// �ش����� �ε��� �ӵ��� �ſ� ���� 
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
