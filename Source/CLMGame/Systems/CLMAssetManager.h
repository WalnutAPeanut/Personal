// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "Templates/SubclassOf.h"
#include "CLMAssetManager.generated.h"


class UPrimaryDataAsset;

class UCLMGameData;
class UCLMPawnData;

/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	UCLMAssetManager();

	// Returns the AssetManager singleton object.
	static UCLMAssetManager& Get();
	static bool ShouldLogAssetLoads();

	static UObject* SynchronousLoadAsset(const FSoftObjectPath& AssetPath);

	virtual void StartInitialLoading() override;


	// Returns the asset referenced by a TSoftObjectPtr.  This will synchronously load the asset if it's not already loaded.
	template<typename AssetType>
	static AssetType* GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// Returns the subclass referenced by a TSoftClassPtr.  This will synchronously load the asset if it's not already loaded.
	template<typename AssetType>
	static TSubclassOf<AssetType> GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory = true);

	// Thread safe way of adding a loaded asset to keep in memory.
	void AddLoadedAsset(const UObject* Asset);


	//// Global game data asset to use.
	//UPROPERTY(Config)
	//TSoftObjectPtr<UCLMGameData> LyraGameDataPath;

	//// Loaded version of the game data
	//UPROPERTY(Transient)
	//TMap<TObjectPtr<UClass>, TObjectPtr<UPrimaryDataAsset>> GameDataMap;

	//// Pawn data used when spawning player pawns if there isn't one set on the player state.
	//UPROPERTY(Config)
	//TSoftObjectPtr<UCLMPawnData> DefaultPawnData;


	// Assets loaded and tracked by the asset manager.
	UPROPERTY()
	TSet<TObjectPtr<const UObject>> LoadedAssets;

	// Used for a scope lock when modifying the list of load assets.
	FCriticalSection LoadedAssetsCritical;
};

template<typename AssetType>
inline AssetType* UCLMAssetManager::GetAsset(const TSoftObjectPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	AssetType* LoadedAsset = nullptr;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedAsset = AssetPointer.Get();
		if (!LoadedAsset)
		{
			LoadedAsset = Cast<AssetType>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedAsset, TEXT("Failed to load asset [%s]"), *AssetPointer.ToString());
		}

		if (LoadedAsset && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedAsset));
		}
	}

	return LoadedAsset;
}

template<typename AssetType>
inline TSubclassOf<AssetType> UCLMAssetManager::GetSubclass(const TSoftClassPtr<AssetType>& AssetPointer, bool bKeepInMemory)
{
	TSubclassOf<AssetType> LoadedSubclass;

	const FSoftObjectPath& AssetPath = AssetPointer.ToSoftObjectPath();

	if (AssetPath.IsValid())
	{
		LoadedSubclass = AssetPointer.Get();
		if (!LoadedSubclass)
		{
			LoadedSubclass = Cast<UClass>(SynchronousLoadAsset(AssetPath));
			ensureAlwaysMsgf(LoadedSubclass, TEXT("Failed to load asset class [%s]"), *AssetPointer.ToString());
		}

		if (LoadedSubclass && bKeepInMemory)
		{
			// Added to loaded asset list.
			Get().AddLoadedAsset(Cast<UObject>(LoadedSubclass));
		}
	}

	return LoadedSubclass;
}
