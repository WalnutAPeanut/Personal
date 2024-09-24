// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;
/**
 * 
 */
struct FCLMGameplayTags
{
	/**
	 * static methods
	 */
	static const FCLMGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

	/**
	 * member methods
	 */
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags(UGameplayTagsManager& Manager);

	/**
	 * �Ʒ��� GameplayTag�� �ʱ�ȭ ���� �ܰ踦 �ǹ��Ѵ�:
	 * - GameInstance�� �ʱ�ȭ ������ UGameFrameworkComponentManager�� RegisterInitState�� ��ϵǾ� ����������(linear)�ϰ� ������Ʈ �ȴ�
	 * - �� �ʱ�ȭ GameplayTag�� ������ Actor ���̿� �����Ǹ�, GameFrameworkInitStateInterface ��ӹ��� Ŭ������ �ʱ�ȭ ����(Init State)�� ���¸ӽ�(State Machine)�� ���� ���� ������ �������̽��� �����Ѵ�
	 */
	FGameplayTag InitState_Spawned;
	FGameplayTag InitState_DataAvailable;
	FGameplayTag InitState_DataInitialized;
	FGameplayTag InitState_GameplayReady;

	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;

private:
	// static ���� �ʱ�ȭ�� .cpp�� ���ִ� ���� ���� ����!
	static FCLMGameplayTags GameplayTags;
};
