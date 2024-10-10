// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CLMAnimInstance.h"
#include "AbilitySystemGlobals.h"

void UCLMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void UCLMAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	// ASC ���� �����ϴ� GameplayTag�� AnimInstance�� ��� Property�� Delegate�� �����Ͽ�, �� ��ȭ�� ���� �ݿ��� �����Ѵ�
	GameplayTagPropertyMap.Initialize(this, ASC);
}
