// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMEquipmentInstance.h"
#include "CLMEquipmentDefinition.h"
#include "GameFramework/Character.h"

UCLMEquipmentInstance::UCLMEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

APawn* UCLMEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

void UCLMEquipmentInstance::SpawnEquipmentActors(const TArray<FCLMEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		// ���� Owner�� Pawn�� RootComponent�� AttachTarget ������� �Ѵ�
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			// ���� ĳ���Ͷ��, SkeletalMeshComponent�� ������ GetMesh�� ��ȯ�Ͽ�, ���⿡ ���δ�
			AttachTarget = Char->GetMesh();
		}

		for (const FCLMEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			// SpawnActorDeferred�� FinishSpawning�� ȣ���ؾ� Spawn�� �ϼ��ȴ� (��, �ۼ��ڿ��� �ڵ�μ� Ownership�� �ִٴ� �ǹ�)
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/true);

			// Actor�� RelativeTransform�� AttachTransform���� ����
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);

			// AttachTarget�� ������ (Actor -> Actor)
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}

void UCLMEquipmentInstance::DestroyEquipmentActors()
{
	// ����� �������� �������� Actor Mesh�� �����Ǿ� ���� ���� �ִ�
	// - ���� Lv10�̾�����, ��ü�� ��ü�� ���� �����Ǿ����� ���� �����ϱ�?
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

APawn* UCLMEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	return Result;
}

void UCLMEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UCLMEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}
