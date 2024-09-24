// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMGameInstance.h"
#include "Components/GameFrameworkComponentManager.h"
#include "CLMGame/CLMGameplayTags.h"

void UCLMGameInstance::Init()
{
	Super::Init();

	// �ռ� ������ InitState�� GameplayTags ���:
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		const FCLMGameplayTags& GameplayTags = FCLMGameplayTags::Get();

		// �ʱ�ȭ ������ ���� �ϴ°�
		//RegisterInitState �Լ��� GameManager Init �Լ� ������ ���Ǹ� �ؾ��� �ʼ�!
		// -> ������Ʈ ���� �ܰ� ������ �����Ҽ����� �ش�κ��� �����ϱ� ���ѹ��.
		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}
}

void UCLMGameInstance::Shutdown()
{
	Super::Shutdown();
}
