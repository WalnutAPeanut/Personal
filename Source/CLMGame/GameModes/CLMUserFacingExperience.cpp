// Fill out your copyright notice in the Description page of Project Settings.

#include "CLMUserFacingExperience.h"
#include "CommonSessionSubsystem.h"

UCommonSession_HostSessionRequest* UCLMUserFacingExperience::CreateHostingRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();

	// ��� �Ѱ��� ���� �����غ���:
	// - UCommonSession_HostSessionRequest�� UObject�� �����س���, �˾Ƽ� GC�� �ȴ�:
	// - �ش� ��ü�� ���� �����ӿ��� ����ϱ� ������, GC�� ���� ������ �ʿ����: ���� ���� �������̵� �߰����� ������ �󿡼� �ش� ��ü�� ����� ���, Lifetime ���� �ʿ�!
	//   - �׷��� ������ dangling ����!
	UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	Result->MapID = MapID;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);

	return Result;
}
