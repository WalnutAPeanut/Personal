#include "CLMGameplayTags.h"
#include "CLMLogChannels.h"
#include "GameplayTagsManager.h"

FCLMGameplayTags FCLMGameplayTags::GameplayTags;

void FCLMGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(Manager);
}

void FCLMGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

void FCLMGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	/**
	 * GameFrameworkComponentManager init state tags
	 */
	AddTag(InitState_Spawned, "InitState.Spawned", "1: Actor/Component has initially spawned and can be extended");
	AddTag(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
	AddTag(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	AddTag(InitState_GameplayReady, "InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");

	AddTag(InputTag_Move, "InputTag.Move", "4: The actor/component is fully ready for active gameplay");
	AddTag(InputTag_Look_Mouse, "InputTag.Look.Mouse", "4: The actor/component is fully ready for active gameplay");
}