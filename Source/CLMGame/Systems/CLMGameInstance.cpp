// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMGameInstance.h"
#include "Components/GameFrameworkComponentManager.h"
#include "CLMGame/CLMGameplayTags.h"

void UCLMGameInstance::Init()
{
	Super::Init();

	// 앞서 정의한 InitState의 GameplayTags 등록:
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);
	if (ensure(ComponentManager))
	{
		const FCLMGameplayTags& GameplayTags = FCLMGameplayTags::Get();

		// 초기화 순서를 정의 하는곳
		//RegisterInitState 함수는 GameManager Init 함수 에서만 정의를 해야함 필수!
		// -> 컴포넌트 생성 단계 순서가 보장할수없어 해당부분을 정의하기 위한방법.
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
