// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMGameState.h"
#include "CLMExperienceManagerComponent.h"

ACLMGameState::ACLMGameState()
{
	ExperienceManagerComponent = CreateDefaultSubobject<UCLMExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
