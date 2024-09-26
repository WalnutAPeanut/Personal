// Fill out your copyright notice in the Description page of Project Settings.

#include "CLMControllerComponent_CharacterParts.h"
#include "CLMPawnComponent_CharacterParts.h"

UCLMControllerComponent_CharacterParts::UCLMControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UCLMControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void UCLMControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

PRAGMA_DISABLE_OPTIMIZATION
UCLMPawnComponent_CharacterParts* UCLMControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// �����غ���, �츮�� �ռ� CLMPawnComponent_CharacterParts�� ��ӹ޴� B_MannequinPawnCosmetics�� �̹� B_Hero_ShooterMannequin�� �߰��Ͽ���.
		// B_MannequinPawnCosmetics�� ��ȯ�Ǳ� ����Ѵ�
		return ControlledPawn->FindComponentByClass<UCLMPawnComponent_CharacterParts>();
	}
	return nullptr;
}
PRAGMA_ENABLE_OPTIMIZATION

void UCLMControllerComponent_CharacterParts::AddCharacterPart(const FCLMCharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}

void UCLMControllerComponent_CharacterParts::AddCharacterPartInternal(const FCLMCharacterPart& NewPart)
{
	FCLMControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	if (UCLMPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

void UCLMControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (UCLMPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FCLMControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}
	CharacterParts.Reset();
}

void UCLMControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	// ���� OldPawn�� ���ؼ��� Character Parts�� ���� ����������
	if (UCLMPawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<UCLMPawnComponent_CharacterParts>() : nullptr)
	{
		for (FCLMControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	// ���ο� Pawn�� ���ؼ� ���� Controller�� ������ �ִ� Character Parts�� �߰�������
	if (UCLMPawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<UCLMPawnComponent_CharacterParts>() : nullptr)
	{
		for (FCLMControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}
