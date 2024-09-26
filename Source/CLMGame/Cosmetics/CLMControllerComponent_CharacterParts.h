// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CLMCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "CLMControllerComponent_CharacterParts.generated.h"

class UCLMPawnComponent_CharacterParts;

/** ControllerComponent�� �����ϴ� Character Parts */
USTRUCT()
struct FCLMControllerCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part�� ���� ����(��Ÿ ������ == MetaData) */
	UPROPERTY(EditAnywhere)
	FCLMCharacterPart Part;

	/** Character Part �ڵ� (������) - Controller�� Possess�ϰ� �ִ� Pawn���� ������(�ν��Ͻ�) Character Part �ڵ鰪 */
	FCLMCharacterPartHandle Handle;
};


UCLASS(meta = (BlueprintSpawnableComponent))
class UCLMControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	UCLMControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UCLMPawnComponent_CharacterParts* GetPawnCustomizer() const;

	UFUNCTION(BlueprintCallable, Category = Cosmetics)
	void AddCharacterPart(const FCLMCharacterPart& NewPart);

	void AddCharacterPartInternal(const FCLMCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	/** UFUNCTION���� �������� ������ AddDynamic�� ���� �ʴ´�! */
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FCLMControllerCharacterPartEntry> CharacterParts;
};