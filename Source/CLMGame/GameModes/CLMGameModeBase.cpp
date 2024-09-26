

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

	// Experience 비동기 로딩을 위한 Delegater를 준비한다.
	UCLMExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UCLMExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// OnExperienceLoaded 등록
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnCLMExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

UClass* ACLMGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	//에셋 데이터 기반으로 설정된값으로 사용하도록 수정
	if (const UCLMPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	// DefaultPawnClass 정의된 값
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
			// FindPawnExtensionComponent 구현
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


	// 우리가 앞서, URL과 함께 ExtraArgs로 넘겼던 정보는 OptionsString에 저정되어 있다.
	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		// Experience의 Value를 가져와서, PrimaryAssetId를 생성해준다: 이때, HakExperienceDefinition의 Class 이름을 사용한다
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UCLMExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromOptions));
	}

	if (!ExperienceId.IsValid())
	{
		//우선 기본만 세팅을 하드코딩으로 한다.
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
	//게임 도중 PawnData가 오버라이드 되었을경우 PawnData는 PlayerState에서 가져오게 됨
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
