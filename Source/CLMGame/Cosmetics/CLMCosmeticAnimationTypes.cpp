// Fill out your copyright notice in the Description page of Project Settings.

#include "CLMCosmeticAnimationTypes.h"

TSubclassOf<UAnimInstance> FCLMAnimLayerSelectionSet::SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FCLMAnimLayerSelectionEntry& Rule : LayerRules)
	{
		if ((Rule.Layer != nullptr) && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Layer;
		}
	}
	return DefaultLayer;
}

USkeletalMesh* FCLMAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
	// MeshRule�� ��ȸ�ϸ�, CosmeticTags �䱸 ���ǿ� �´� MeshRule�� ã�� SkeletalMesh�� ��ȯ�Ѵ�
	for (const FCLMAnimBodyStyleSelectionEntry& Rule : MeshRules)
	{
		if ((Rule.Mesh) && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Mesh;
		}
	}

	return DefaultMesh;
}
