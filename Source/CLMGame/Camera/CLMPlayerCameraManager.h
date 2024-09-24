// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CLMPlayerCameraManager.generated.h"

/**
 * Controller에 바인딩된 CameraManager
 */
#define CLM_CAMERA_DEFAULT_FOV (80.0f)
#define CLM_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define CLM_CAMERA_DEFAULT_PITCH_MAX (89.0f)

/**
 * 
 */
UCLASS()
class CLMGAME_API ACLMPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	ACLMPlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
