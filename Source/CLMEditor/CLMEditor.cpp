// Copyright Epic Games, Inc. All Rights Reserved.

#include "CLMEditor.h"

#define LOCTEXT_NAMESPACE "CLMEditor"

DEFINE_LOG_CATEGORY(LogCLMEditor);

/**
 * FCLMEditorModule
 */
class FCLMEditorModule : public FDefaultGameModuleImpl
{
	typedef FCLMEditorModule ThisClass;

	virtual void StartupModule() override
	{
		//FGameEditorStyle::Initialize();

		//if (!IsRunningGame())
		//{
		//	FModuleManager::Get().OnModulesChanged().AddRaw(this, &FCLMEditorModule::ModulesChangedCallback);

		//	BindGameplayAbilitiesEditorDelegates();

		//	if (FSlateApplication::IsInitialized())
		//	{
		//		ToolMenusHandle = UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateStatic(&RegisterGameEditorMenus));
		//	}

		//	FEditorDelegates::BeginPIE.AddRaw(this, &ThisClass::OnBeginPIE);
		//	FEditorDelegates::EndPIE.AddRaw(this, &ThisClass::OnEndPIE);
		//}

		//// Register the Context Effects Library asset type actions.
		//{
		//	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		//	TSharedRef<FAssetTypeActions_CLMContextEffectsLibrary> AssetAction = MakeShared<FAssetTypeActions_CLMContextEffectsLibrary>();
		//	CLMContextEffectsLibraryAssetAction = AssetAction;
		//	AssetTools.RegisterAssetTypeActions(AssetAction);
		//}
	}

	/*void OnBeginPIE(bool bIsSimulating)
	{
		UCLMExperienceManager* ExperienceManager = GEngine->GetEngineSubsystem<UCLMExperienceManager>();
		check(ExperienceManager);
		ExperienceManager->OnPlayInEditorBegun();
	}

	void OnEndPIE(bool bIsSimulating)
	{
	}*/

	virtual void ShutdownModule() override
	{
		//// Unregister the Context Effects Library asset type actions.
		//{
		//	FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
		//	TSharedPtr<IAssetTypeActions> AssetAction = CLMContextEffectsLibraryAssetAction.Pin();
		//	if (AssetToolsModule && AssetAction)
		//	{
		//		AssetToolsModule->Get().UnregisterAssetTypeActions(AssetAction.ToSharedRef());
		//	}
		//}

		//FEditorDelegates::BeginPIE.RemoveAll(this);
		//FEditorDelegates::EndPIE.RemoveAll(this);

		//// Undo UToolMenus
		//if (UObjectInitialized() && ToolMenusHandle.IsValid())
		//{
		//	UToolMenus::UnRegisterStartupCallback(ToolMenusHandle);
		//}

		//UnbindGameplayAbilitiesEditorDelegates();
		//FModuleManager::Get().OnModulesChanged().RemoveAll(this);
		//FGameEditorStyle::Shutdown();
	}

protected:

	/*static void BindGameplayAbilitiesEditorDelegates()
	{
		IGameplayAbilitiesEditorModule& GameplayAbilitiesEditorModule = IGameplayAbilitiesEditorModule::Get();

		GameplayAbilitiesEditorModule.GetGameplayCueNotifyClassesDelegate().BindStatic(&GetGameplayCueDefaultClasses);
		GameplayAbilitiesEditorModule.GetGameplayCueInterfaceClassesDelegate().BindStatic(&GetGameplayCueInterfaceClasses);
		GameplayAbilitiesEditorModule.GetGameplayCueNotifyPathDelegate().BindStatic(&GetGameplayCuePath);
	}*/

	/*static void UnbindGameplayAbilitiesEditorDelegates()
	{
		if (IGameplayAbilitiesEditorModule::IsAvailable())
		{
			IGameplayAbilitiesEditorModule& GameplayAbilitiesEditorModule = IGameplayAbilitiesEditorModule::Get();
			GameplayAbilitiesEditorModule.GetGameplayCueNotifyClassesDelegate().Unbind();
			GameplayAbilitiesEditorModule.GetGameplayCueInterfaceClassesDelegate().Unbind();
			GameplayAbilitiesEditorModule.GetGameplayCueNotifyPathDelegate().Unbind();
		}
	}

	void ModulesChangedCallback(FName ModuleThatChanged, EModuleChangeReason ReasonForChange)
	{
		if ((ReasonForChange == EModuleChangeReason::ModuleLoaded) && (ModuleThatChanged.ToString() == TEXT("GameplayAbilitiesEditor")))
		{
			BindGameplayAbilitiesEditorDelegates();
		}
	}*/

private:
	/*TWeakPtr<IAssetTypeActions> CLMContextEffectsLibraryAssetAction;
	FDelegateHandle ToolMenusHandle;*/
};

IMPLEMENT_MODULE(FCLMEditorModule, CLMEditor);