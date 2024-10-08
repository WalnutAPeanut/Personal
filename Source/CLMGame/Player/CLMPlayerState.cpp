// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMPlayerState.h"
#include "CLMPlayerController.h"
#include "CLMGame/GameModes/CLMExperienceManagerComponent.h"
#include "CLMGame/GameModes/CLMGameModeBase.h"
#include "CLMGame/AbilitySystem/CLMAbilitySystemComponent.h"

ACLMPlayerState::ACLMPlayerState(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UCLMAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	//AbilitySystemComponent->SetIsReplicated(true);
	//AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

}

ACLMPlayerController* ACLMPlayerState::GetCLMPlayerController() const
{
	return Cast<ACLMPlayerController>(GetOwner());
}

UAbilitySystemComponent* ACLMPlayerState::GetAbilitySystemComponent() const
{
	return GetCLMAbilitySystemComponent();
}

void ACLMPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UCLMExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UCLMExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnCLMExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ACLMPlayerState::OnExperienceLoaded(const UCLMExperienceDefinition* CurrentExperience)
{
	if (ACLMGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ACLMGameModeBase>())
	{
		// AHakGameMode에서 GetPawnDataForController를 구현해야 함
		// - GetPawnDataForController에서 우리는 아직 PawnData를 설정하지 않았으므로, ExperienceMangerComponent의 DefaultPawnData로 설정한다
		const UCLMPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void ACLMPlayerState::SetPawnData(const UCLMPawnData* InPawnData)
{
	check(InPawnData);

	// PawnData가 두번 설정되는 것은 원하지 않음!
	check(!PawnData);

	PawnData = InPawnData;
}
