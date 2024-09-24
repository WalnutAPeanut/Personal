// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMPlayerCameraManager.h"

ACLMPlayerCameraManager::ACLMPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	DefaultFOV = CLM_CAMERA_DEFAULT_FOV;
	ViewPitchMin = CLM_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = CLM_CAMERA_DEFAULT_PITCH_MAX;
}
