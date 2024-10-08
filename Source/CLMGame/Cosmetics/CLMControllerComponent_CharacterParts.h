// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CLMCharacterPartTypes.h"
#include "Components/ControllerComponent.h"
#include "CLMControllerComponent_CharacterParts.generated.h"

class UCLMPawnComponent_CharacterParts;

/** ControllerComponent가 소유하는 Character Parts */
USTRUCT()
struct FCLMControllerCharacterPartEntry
{
	GENERATED_BODY()

	/** Character Part에 대한 정의(메타 데이터 == MetaData) */
	UPROPERTY(EditAnywhere)
	FCLMCharacterPart Part;

	/** Character Part 핸들 (고유값) - Controller가 Possess하고 있는 Pawn에서 생성한(인스턴스) Character Part 핸들값 */
	FCLMCharacterPartHandle Handle;
};


UCLASS(meta = (BlueprintSpawnableComponent))
class UCLMControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	UCLMControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Cosmetics)
	void AddCharacterPart(const FCLMCharacterPart& NewPart);

	/*UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Cosmetics)
	void RemoveCharacterPart(const FCLMCharacterPart& PartToRemove);*/
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Cosmetics)
	void RemoveAllCharacterParts();

private:
	UCLMPawnComponent_CharacterParts* GetPawnCustomizer() const;

	void AddCharacterPartInternal(const FCLMCharacterPart& NewPart);

	/** UFUNCTION으로 정의하지 않으면 AddDynamic이 되지 않는다! */
	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

protected:
	UPROPERTY(EditAnywhere, Category = Cosmetics)
	TArray<FCLMControllerCharacterPartEntry> CharacterParts;
};