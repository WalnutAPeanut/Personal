// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Equipment/CLMGameplayAbility_FromEquipment.h"
#include "CLMGameplayAbility_RangedWeapon.generated.h"

class UCLMRangedWeaponInstance;
/**
 * 
 */
UCLASS()
class CLMGAME_API UCLMGameplayAbility_RangedWeapon : public UCLMGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:
	UCLMGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// 총알에 대한 정보(날라가는 방향, 탄피,)
	struct FRangedWeaponFiringInput
	{
		// Start of the trace
		FVector StartTrace;

		// End of the trace if aim were perfect
		FVector EndAim;

		// The direction of the trace if aim were perfect
		FVector AimDir;

		// The weapon instance / source of weapon data
		UCLMRangedWeaponInstance* WeaponData = nullptr;

		// Can we play bullet FX for hits during this trace
		bool bCanPlayBulletFX = false;

		FRangedWeaponFiringInput()
			: StartTrace(ForceInitToZero)
			, EndAim(ForceInitToZero)
			, AimDir(ForceInitToZero)
		{
		}
	};
protected:
	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();
};
