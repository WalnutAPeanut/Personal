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

	// ���� �ռ� ������ �Ǿ�� �Ǵ� Component
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

	// Pawn�� Possess�μ�, Controller�� PlayerState ���� ������ ������ ���°� �Ǿ���:
	// - SetupPlayerInputComponent Ȯ��
	PawnExtComponent->SetupPlayerInputComponent();
}

