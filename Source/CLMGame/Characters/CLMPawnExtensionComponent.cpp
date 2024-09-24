// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "CLMGame/CLMGameplayTags.h"
#include "CLMGame/CLMLogChannels.h"
#include "CLMGame/Characters/CLMPawnData.h"

/** feature name�� component �����ϱ� component�� ���� pawn extension�� ���� ���� Ȯ���� �� �ִ� */
const FName UCLMPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UCLMPawnExtensionComponent::UCLMPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UCLMPawnExtensionComponent::SetPawnData(const UCLMPawnData* InPawnData)
{
	// Pawn�� ���� Authority�� ���� ���, SetPawnData�� �������� ����
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		return;
	}

	// PawnData ������Ʈ
	PawnData = InPawnData;

	CheckDefaultInitialization();
}

void UCLMPawnExtensionComponent::SetupPlayerInputComponent()
{
	// ForceUpdate�� �ٽ� InitState ���� ��ȯ ���� (���� ��)
	CheckDefaultInitialization();
}

void UCLMPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	// �ùٸ� Actor�� ��ϵǾ����� Ȯ��:
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogCLM, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	// GameFrameworkComponentManager�� InitState ����� ���� ��� ����:
	// - ����� ��ӹ޾Ҵ� IGameFrameworkInitStateInterface �޼��� RegisterInitStateFeature()�� Ȱ��
	// - �ش� �Լ��� ������ ����
	RegisterInitStateFeature();

	// ������� ���� �Լ�
	UGameFrameworkComponentManager* Manager = UGameFrameworkComponentManager::GetForActor(GetOwningActor());
}

void UCLMPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();
	// FeatureName�� NAME_None�� ������, Actor�� ��ϵ� Feature Component�� InitState ���¸� �����ϰڴٴ� �ǹ�:
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// InitState_Spawned�� ���� ��ȯ:
	// - TryToChangeInitState�� �Ʒ��� ���� ����ȴ�:
	//   1. CanChangeInitState�� ���� ��ȯ ���ɼ� ���� �Ǵ�
	//   2. HandleChangeInitState�� ���� ���� ���� (Feature Component)
	//   3. BindOnActorInitStateChanged�� Bind�� Delegate�� ���ǿ� �°� ȣ���� ��
	//      - CLMPawnExtensionComponent�� ���, ��� Actor�� Feature ���� ��ȭ�� ���� OnActorInitStateChanged()�� ȣ���
	ensure(TryToChangeInitState(FCLMGameplayTags::Get().InitState_Spawned));

	// �ش� �Լ��� �츮�� �������̵� �Ѵ�:
	// - �� �Լ��� ForceUpdateInitState�� ���� �������� �������ָ� �ȴ� 
	// - ���� ���� ������Ʈ ���� (���� CanChangeInitState�� HandleChangleInitState�� �������ش�)
	CheckDefaultInitialization();
}

void UCLMPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// �ռ�, OnRegister�� RegisterInitStateFeature()�� ���� ���߾�����
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UCLMPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		// CLMPawnExtensionComponent�� �ٸ� Feature Component���� ���°� DataAvailable�� �����Ͽ�, Sync�� ���ߴ� ������ �־��� (CanChangeInitState)
		// - �̸� �������ϱ� ����, OnActorInitStateChanged������ DataAvailable�� ���� ���������� CheckDefaultInitialization�� ȣ���Ͽ�, ���¸� Ȯ���Ѵ�
		const FCLMGameplayTags& InitTags = FCLMGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UCLMPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);
	

	APawn* Pawn = GetPawn<APawn>();
	const FCLMGameplayTags& InitTags = FCLMGameplayTags::Get();

	// InitState_Spawned �ʱ�ȭ
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// Pawn�� �� ���ø� �Ǿ������� �ٷ� Spawned�� �Ѿ!
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// PawnData�� GameMode SpawnDefaultPawnAtTransform_Implementation ���� ������ �ȴ�.
		if (!PawnData)
		{
			return false;
		}

		// LocallyControlled�� Pawn�� Controller�� ������ ����!
		//const bool bHasAuthority = Pawn->HasAuthority();
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

		//if (bHasAuthority || bIsLocallyControlled)
		if (bIsLocallyControlled)
		{
			// Check for being possessed by a controller.
			if (!GetController<AController>())
			{
				return false;
			}
		}
		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Actor�� ���ε�� ��� Feature���� DataAvailable ������ ��, DataInitialized�� �Ѿ:
		// - HaveAllFeaturesReachedInitState Ȯ��
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);	
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	// ���� ��������(linear) transition�� �ƴϸ� false!
	return false;
}

void UCLMPawnExtensionComponent::CheckDefaultInitialization()
{
	// PawnExtensionComponent�� Feature Component���� �ʱ�ȭ�� �����ϴ� Component�̴�:
	// - ����, Actor�� ���ε��� Feature Component�鿡 ���� CheckDefaultInitialization�� ȣ�����ֵ��� �Ѵ� (ForceUpdate ����?)
	// - �� �޼��带 IGameFrameworkInitStateInterface�� �����ϴµ�, CheckDefaultInitializationForImplementers�̴�:
	// - ������ CheckDefaultInitializationForImplementers ����:
	CheckDefaultInitializationForImplementers();

	const FCLMGameplayTags& InitTags = FCLMGameplayTags::Get();

	// ����� ���� InitState�� ���� �Ѱ���� �Ѵ�... (��� �̰� �� �˾Ƽ� �� �� ������ ������ ����...)
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };

	// CanChangeInitState()�� HandleChangeInitState() �׸��� ChangeFeatureInitState ȣ���� ���� OnActorInitStateChanged Delegate ȣ����� �������ش�:
	// �Ʒ��� �ڵ带 ������ ����:
	// - �ش� �ڵ���� ����, �츮�� ������ ������ �� �ִ�:
	//   - ��� ���¸� �����Ҷ����� �ѹ� ������Ʈ �Ѵ�
	//   - InitState�� ���� ��ȭ�� Linear(������)����!... -> ���߿� Diagram���� �����ϸ鼭 ����
	//     - ������Ʈ�� ���߸� ������ ��������� ���� �ǹ�! (chaining)
	auto CurrentState = ContinueInitStateChain(StateChain);
	UE_LOG(LogCLM, Log, TEXT("UCLMPawnExtensionComponent::CheckDefaultInitialization: %s"), *CurrentState.ToString());
}
