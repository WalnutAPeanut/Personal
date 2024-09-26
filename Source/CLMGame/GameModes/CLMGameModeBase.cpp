

#include "CLMGameModeBase.h"
#include "CLMExperienceManagerComponent.h"
#include "CLMGameState.h"
#include "CLMGame/Player/CLMPlayerState.h"
#include "CLMGame/Player/CLMPlayerController.h"
#include "CLMGame/Characters/CLMCharacter.h"
#include "CLMGame/CLMLogChannels.h"
#include "CLMGame/Characters/CLMPawnData.h"
#include "CLMGame/Characters/CLMPawnExtensionComponent.h"
#include "CLMGame/GameModes/CLMExperienceDefinition.h"
#include "Kismet/GameplayStatics.h"

ACLMGameModeBase::ACLMGameModeBase()
{
	GameStateClass = ACLMGameState::StaticClass();
	PlayerControllerClass = ACLMPlayerController::StaticClass();
	PlayerStateClass = ACLMPlayerState::StaticClass();
	DefaultPawnClass = ACLMCharacter::StaticClass();
}

void ACLMGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ACLMGameModeBase::InitGameState()
{
	Super::InitGameState();

	// Experience �񵿱� �ε��� ���� Delegater�� �غ��Ѵ�.
	UCLMExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UCLMExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// OnExperienceLoaded ���
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnCLMExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* ACLMGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	//���� ������ ������� �����Ȱ����� ����ϵ��� ����
	if (const UCLMPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	// DefaultPawnClass ���ǵ� ��
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ACLMGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* ACLMGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			// FindPawnExtensionComponent ����
			if (UCLMPawnExtensionComponent* PawnExtComp = UCLMPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const UCLMPawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}

	return nullptr;
}

void ACLMGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	
	UWorld* World = GetWorld();


	// �츮�� �ռ�, URL�� �Բ� ExtraArgs�� �Ѱ�� ������ OptionsString�� �����Ǿ� �ִ�.
	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		// Experience�� Value�� �����ͼ�, PrimaryAssetId�� �������ش�: �̶�, HakExperienceDefinition�� Class �̸��� ����Ѵ�
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UCLMExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromOptions));
	}

	if (!ExperienceId.IsValid())
	{
		//�켱 �⺻�� ������ �ϵ��ڵ����� �Ѵ�.
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("CLMExperienceDefinition"), FName("B_CLMDefaultExperience"));
	}

	OnMatchAssignmentGiven(ExperienceId);
}

void ACLMGameModeBase::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	check(ExperienceId.IsValid());

	UCLMExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UCLMExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->ServerSetCurrentExperience(ExperienceId);
}

bool ACLMGameModeBase::IsExperienceLoaded()
{
	check(GameState);
	UCLMExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UCLMExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	return ExperienceManagerComponent->IsExperienceLoaded();
}

void ACLMGameModeBase::OnExperienceLoaded(const UCLMExperienceDefinition* CurrentExperience)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);

		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

const UCLMPawnData* ACLMGameModeBase::GetPawnDataForController(const AController* InController) const
{
	//���� ���� PawnData�� �������̵� �Ǿ������ PawnData�� PlayerState���� �������� ��
	if (InController)
	{
		if (const ACLMPlayerState* CLMPS = InController->GetPlayerState<ACLMPlayerState>())
		{
			if (const UCLMPawnData* PawnData = CLMPS->GetPawnData<UCLMPawnData>())
			{
				return PawnData;
			}
		}
	}

	check(GameState);
	UCLMExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UCLMExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		const UCLMExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}
	return nullptr;
}
