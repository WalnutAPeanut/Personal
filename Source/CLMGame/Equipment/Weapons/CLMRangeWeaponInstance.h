// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Weapons/CLMWeaponInstance.h"
#include "CLMRangeWeaponInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMRangeWeaponInstance : public UCLMWeaponInstance
{
	GENERATED_BODY()
public:
	/** À¯È¿ »ç°Å¸® */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float MaxDamageRange = 25000.0f;

	/** ÃÑÅº ¹üÀ§ (Sphere Trace Sweep) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponConfig", meta = (ForceUnits = cm))
	float BulletTraceWeaponRadius = 0.0f;
};
