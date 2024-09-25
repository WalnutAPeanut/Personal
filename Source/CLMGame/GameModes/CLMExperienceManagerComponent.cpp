// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMExperienceManagerComponent.h"
#include "CLMGame/CLMLogChannels.h"
#include "CLMGame/Systems/CLMAssetManager.h"
#include "CLMGame/GameModes/CLMExperienceDefinition.h"
#include "CLMGame/GameModes/CLMExperienceActionSet.h"
#include "GameFeaturesSubsystemSettings.h"
#include "GameFeaturesSubsystem.h"
#include "GameFeatureAction.h"

void UCLMExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnCLMExperienceLoaded::FDelegate&& Delegate)
{
	
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		/// Delegate ��ü�� ���������� �ʿ��� �������� �޸� �Ҵ��� ���´�.
		/// TArray<int> a = {1,2,3,4};
		/// delegate_type delegate = [a]()
		/// {
		///		return a.Num();
		/// }
		/// A�� delegate_type ���ο� new�� �Ҵ�Ǿ� �ִ�. ���� ����� ���߱� ���� Move�� ���� �ϴ°��� ���� ����!!
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void UCLMExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	UCLMAssetManager& AssetManager = UCLMAssetManager::Get();

	TSubclassOf<UCLMExperienceDefinition> AssetClass;
	FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
	AssetClass = Cast<UClass>(AssetPath.TryLoad());

	//�� CDO�� �������� �ɱ�?
	const UCLMExperienceDefinition* Experience = GetDefault<UCLMExperienceDefinition>(AssetClass);
	check(Experience);
	check(CurrentExperience == nullptr);
	// �׸��� CDO�� CurrentExperience�� �����Ѵ�!
	// ��� �ǵ��� �̷��� �ڵ带 �ۼ������� �ڵ带 �� �о��(StartExperienceLoad ���� �о��)
	CurrentExperience = Experience;
	StartExperienceLoad();
}

void UCLMExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == ECLMExperienceLoadState::Unloaded);

	LoadState = ECLMExperienceLoadState::Loading;

	UCLMAssetManager& AssetManager = UCLMAssetManager::Get();

	// �̹� �ռ�, ServerSetCurrentExperience���� �츮�� ExperienceId�� �Ѱ��־��µ�, ���⼭ CDO�� Ȱ���Ͽ�, GetPrimaryAssetId�� �ε��� ������� �ִ´�!
	// - �� �̷��� �ϴ°ɱ�?
	// - GetPrimaryAssetId�� �� �� �ڼ�������:
	// - GetPrimaryAssetId�� ���캽���ν�, �Ʒ��� �ΰ����� �� �� �ִ�:
	//   1. �츮�� B_CLMDefaultExperience�� BP�� ���� ����
	//   2. CDO�� �����ͼ�, GetPrimaryAssetId�� ȣ���� ����

	// �츮�� �ռ� �̹� CDO�� �ε��Ͽ�, CDO�� ������� �ʰ� CDO�� ����Ͽ� �ε��� ������ �����Ͽ�, BundleAssetList�� �߰����ش�!

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	// load assets associated with the experience
	// �Ʒ��� �츮�� ���� GameFeature�� ����Ͽ�, Experience�� ���ε��� GameFeature Plugin�� �ε��� Bundle �̸��� �߰��Ѵ�:
	// - Bundle�̶�°� ���� �츮�� �ε��� ������ ī�װ� �̸��̶�� �����ϸ� �ȴ� (�ϴ� ������ �Ѿ�� ����, �� �ٷ� ���̴�!)
	TArray<FName> BundlesToLoad;
	{
		// ���⼭ �ָ��ؾ� �� �κ��� OwnerNetMode�� NM_Standalone�̸�? Client/Server �Ѵ� �ε��� �߰��ȴ�!
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);

	// �Ʒ���, ���� Bundle�� �츮�� GameFeature�� �����ϸ鼭 �� ��� �˾ƺ���� �ϰ�, ������ �ռ� B_CLMDefaultExperience�� �ε����ִ� �Լ��� ��������
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// �ε��� �Ϸ�Ǿ�����, ExecuteDelegate�� ���� OnAssetsLoadedDelegate�� ȣ������:
		// - �Ʒ��� �Լ��� Ȯ���غ���:
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}

	// FrameNumber�� �ָ��ؼ� ����
	static int32 StartExperienceLoad_FrameNumber = GFrameNumber;
}

void UCLMExperienceManagerComponent::OnExperienceLoadComplete()
{
	//FrameNuber��??
	static int32 OnExperienceLoadCompleate_FrameNuber = GFrameNumber;

	check(LoadState == ECLMExperienceLoadState::Loading);
	check(CurrentExperience);

	// ���� Ȱ��ȭ�� GameFeature Plugin�� URL�� Ŭ�������ش�
	GameFeaturePluginURLs.Reset();

	auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
		{
			// FeaturePluginList�� ��ȸ�ϸ�, PluginURL�� ExperienceManagerComponent�� GameFeaturePluginURLS�� �߰����ش�
			for (const FString& PluginName : FeaturePluginList)
			{
				FString PluginURL;
				if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
				{
					This->GameFeaturePluginURLs.AddUnique(PluginURL);
				}
			}
		};

	// GameFeaturesToEnable�� �ִ� Plugin�� �ϴ� Ȱ��ȭ�� GameFeature Plugin �ĺ������� ���
	CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);

	// GameFeaturePluginURLs�� ��ϵ� Plugin�� �ε� �� Ȱ��ȭ:
	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading)
	{
		LoadState = ECLMExperienceLoadState::LoadingGameFeatures;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			// �� Plugin�� �ε� �� Ȱ��ȭ ����, OnGameFeaturePluginLoadComplete �ݹ� �Լ� ���
			// �ش� �Լ��� ���캸���� ����
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		// �ش� �Լ��� �Ҹ��� ���� �ռ� ���Ҵ� StreamableDelegateDelayHelper�� ���� �Ҹ�
		OnExperienceFullLoadCompleted();
	}


}

void UCLMExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	// �� GameFeature Plugin�� �ε��� ��, �ش� �Լ��� �ݹ����� �Ҹ���
	NumGameFeaturePluginsLoading--;
	if (NumGameFeaturePluginsLoading == 0)
	{
		// GameFeaturePlugin �ε��� �� ������ ���, ������� Loaded�μ�, OnExperienceFullLoadCompleted ȣ���Ѵ�
		// GameFeaturePlugin �ε��� Ȱ��ȭ�� �����ٸ�? UGameFeatureAction�� Ȱ��ȭ�ؾ߰��� (���ݸ� �ִٰ� ����)
		OnExperienceFullLoadCompleted();
	}
}

void UCLMExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != ECLMExperienceLoadState::Loaded);

	// GameFeature Plugin�� �ε��� Ȱ��ȭ ����, GameFeature Action���� Ȱ��ȭ ��Ű��:
	LoadState = ECLMExperienceLoadState::ExecutingActions;

	// GameFeatureAction Ȱ��ȭ�� ���� Context �غ�
	FGameFeatureActivatingContext Context;
	// ������ �ڵ��� �������ش�
	const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
	if (ExistingWorldContext)
	{
		Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
	}

	auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
		{
			for (UGameFeatureAction* Action : ActionList)
			{
				// ��������� GameFeatureAction�� ���� Registering -> Loading -> Activating ������ ȣ���Ѵ�
				if (Action)
				{
					Action->OnGameFeatureRegistering();
					Action->OnGameFeatureLoading();
					Action->OnGameFeatureActivating(Context);
				}
			}
		};

	// 1. Experience�� Actions
	ActivateListOfActions(CurrentExperience->Actions);

	// 2. Experience�� ActionSets
	for (const TObjectPtr<UCLMExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
	{
		ActivateListOfActions(ActionSet->Actions);
	}

	LoadState = ECLMExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const UCLMExperienceDefinition* UCLMExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == ECLMExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);
	return CurrentExperience;
}
