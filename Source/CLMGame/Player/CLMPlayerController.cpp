// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMPlayerController.h"
#include "CLMGame/Player/CLMPlayerState.h"
#include "CLMGame/AbilitySystem/CLMAbilitySystemComponent.h"
#include "CLMGame/Camera/CLMPlayerCameraManager.h"

ACLMPlayerController::ACLMPlayerController(const FObjectInitializer& ObjectInitializer)
{
	PlayerCameraManagerClass = ACLMPlayerCameraManager::StaticClass();
}

ACLMPlayerState* ACLMPlayerController::GetCLMPlayerState() const
{
	return CastChecked<ACLMPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UCLMAbilitySystemComponent* ACLMPlayerController::GetCLMAbilitySystemComponent() const
{
	const ACLMPlayerState* CLMPS = GetCLMPlayerState();
	return (CLMPS ? CLMPS->GetCLMAbilitySystemComponent() : nullptr);
}

void ACLMPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void ACLMPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UCLMAbilitySystemComponent* CLMASC = GetCLMAbilitySystemComponent())
	{
		CLMASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}
