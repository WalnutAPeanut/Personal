// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMCharacter.h"
#include "CLMPawnExtensionComponent.h"
#include "CLMGame/Camera/CLMCameraComponent.h"
#include "AbilitySystem/CLMAbilitySystemComponent.h"

// Sets default values
ACLMCharacter::ACLMCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// 가장 먼서 생성이 되어야 되는 Component
	PawnExtComponent = CreateDefaultSubobject<UCLMPawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	CameraComponent = CreateDefaultSubobject<UCLMCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
}

UAbilitySystemComponent* ACLMCharacter::GetAbilitySystemComponent() const
{
	return PawnExtComponent->GetCLMAbilitySystemComponent();
}

// Called when the game starts or when spawned
void ACLMCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACLMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACLMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn이 Possess로서, Controller와 PlayerState 정보 접근이 가능한 상태가 되었음:
	// - SetupPlayerInputComponent 확인
	PawnExtComponent->SetupPlayerInputComponent();
}

