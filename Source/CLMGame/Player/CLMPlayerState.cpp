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
		// AHakGameMode���� GetPawnDataForController�� �����ؾ� ��
		// - GetPawnDataForController���� �츮�� ���� PawnData�� �������� �ʾ����Ƿ�, ExperienceMangerComponent�� DefaultPawnData�� �����Ѵ�
		const UCLMPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void ACLMPlayerState::SetPawnData(const UCLMPawnData* InPawnData)
{
	check(InPawnData);

	// PawnData�� �ι� �����Ǵ� ���� ������ ����!
	check(!PawnData);

	PawnData = InPawnData;
}
