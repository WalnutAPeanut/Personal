// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMPlayerController.h"
#include "CLMGame/Camera/CLMPlayerCameraManager.h"

ACLMPlayerController::ACLMPlayerController(const FObjectInitializer& ObjectInitializer)
{
	PlayerCameraManagerClass = ACLMPlayerCameraManager::StaticClass();
}

void ACLMPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	int a = 10;
}
