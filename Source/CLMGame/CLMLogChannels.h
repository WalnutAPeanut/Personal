// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/UnrealString.h"
#include "Logging/LogMacros.h"

/// CLMGAME_API 사용하는 이유
/// 로그 카테고리를 다른 모듈에서도 사용할 수 있도록 하기 위해 외부에 공개하는 것입니다.
CLMGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogCLM, Log, All);
CLMGAME_API DECLARE_LOG_CATEGORY_EXTERN(LogCLMExperience, Log, All);