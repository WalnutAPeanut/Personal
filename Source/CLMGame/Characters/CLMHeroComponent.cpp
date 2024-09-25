// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMHeroComponent.h"
#include "CLMGame/CLMLogChannels.h"
#include "CLMGame/CLMGameplayTags.h"
#include "CLMGame/Characters/CLMPawnExtensionComponent.h"
#include "CLMGame/Characters/CLMPawnData.h"
#include "CLMGame/Player/CLMPlayerState.h"
#include "CLMGame/Player/CLMPlayerController.h"
#include "CLMGame/Camera/CLMCameraComponent.h"
#include "CLMGame/Camera/CLMCameraComponent.h"
//#include "CLMGame/Input/CLMMappableConfigPair.h"
#include "CLMGame/Input/CLMInputComponent.h"

#include "Components/GameFrameworkComponentManager.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"

const FName UCLMHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UCLMHeroComponent::NAME_ActorFeatureName("Hero");

UCLMHeroComponent::UCLMHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UCLMHeroComponent::OnRegister()
{
	Super::OnRegister();

	// �ùٸ� Actor�� ��ϵǾ����� Ȯ��:
	if (!GetPawn<APawn>())
	{
		UE_LOG(LogCLM, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
		return;
	}

	RegisterInitStateFeature();
}

void UCLMHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// PawnExtensionComponent�� ���ؼ� (PawnExtension Feature) OnActorInitStateChanged() �����ϵ��� (Observing)
	BindOnActorInitStateChanged(UCLMPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// InitState_Spawned�� �ʱ�ȭ
	ensure(TryToChangeInitState(FCLMGameplayTags::Get().InitState_Spawned));

	// ForceUpdate ����
	CheckDefaultInitialization();
}

void UCLMHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UCLMHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UCLMPawnExtensionComponent::NAME_ActorFeatureName)
	{
		const FCLMGameplayTags& InitTags = FCLMGameplayTags::Get();
		// CLMPawnExtensionComponent�� DataInitialized ���� ��ȭ ���� ��, CLMHeroComponent�� DataInitialized ���·� ����
		// - CanChangeInitState Ȯ��
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UCLMHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FCLMGameplayTags& InitTags = FCLMGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	ACLMPlayerState* CLMPS = GetPlayerState<ACLMPlayerState>();

	// Spawned �ʱ�ȭ
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!CLMPS)
		{
			return false;
		}
		return true;
	}

	// DataAvailable -> DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// PawnExtensionComponent�� DataInitialized�� ������ ��ٸ� (== ��� Feature Component�� DataAvailable�� ����)
		return CLMPS && Manager->HasFeatureReachedInitState(Pawn, UCLMPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_GameplayReady);
		//return CLMPS && Manager->HasFeatureReachedInitState(Pawn, UCLMPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}

	return false;
}

void UCLMHeroComponent::CheckDefaultInitialization()
{
	// �ռ� BindOnActorInitStateChanged���� ���ҵ��� Hero Feature�� Pawn Extension Feature�� ���ӵǾ� �����Ƿ�, CheckDefaultInitializationForImplementers ȣ������ ����:

	// ContinueInitStateChain�� �ռ� PawnExtComponent�� ����
	const FCLMGameplayTags& InitTags = FCLMGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	auto CurrentState = ContinueInitStateChain(StateChain);
	UE_LOG(LogCLM, Log, TEXT("UCLMHeroComponent::CheckDefaultInitialization: %s"), *CurrentState.ToString());
}

void UCLMHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FCLMGameplayTags& InitTags = FCLMGameplayTags::Get();

	// DataAvailable -> DataInitialized �ܰ�
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ACLMPlayerState* CLMPS = GetPlayerState<ACLMPlayerState>();
		if (!ensure(Pawn && CLMPS))
		{
			return;
		}

		const UCLMPawnData* PawnData = nullptr;
		if (UCLMPawnExtensionComponent* PawnExtComp = UCLMPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UCLMPawnData>();

			// DataInitialized �ܰ���� ����, Pawn�� Controller�� Possess�Ǿ� �غ�� �����̴�:
			// - InitAbilityActorInfo ȣ��� AvatarActor �缳���� �ʿ��ϴ�
			//PawnExtComp->InitializeAbilitySystem(CLMPS->GetCLMAbilitySystemComponent(), CLMPS);
		}

		// Input �ڵ鸵
		if (ACLMPlayerController* CLMPC = GetController<ACLMPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		if (PawnData)
		{
			// Camera �ڵ鸵
			// ���� CLMCharacter�� Attach�� CameraComponent�� ã��
			if (UCLMCameraComponent* CameraComponent = UCLMCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}


void UCLMHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	// LocalPlayer�� �������� ����
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	// EnhancedInputLocalPlayerSubsystem �������� ����
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// EnhancedInputLocalPlayerSubsystem�� MappingContext�� ����ش�:
	Subsystem->ClearAllMappings();

	// PawnExtensionComponent -> PawnData -> InputConfig ���� ���� �Ǵ�:
	if (const UCLMPawnExtensionComponent* PawnExtComp = UCLMPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UCLMPawnData* PawnData = PawnExtComp->GetPawnData<UCLMPawnData>())
		{
			if (const UCLMInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FCLMGameplayTags& GameplayTags = FCLMGameplayTags::Get();

				// HeroComponent ������ �ִ� Input Mapping Context�� ��ȸ�ϸ�, EnhancedInputLocalPlayerSubsystem�� �߰��Ѵ�
				for (const FCLMMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						// ���������� Input Mapping Context�� �߰��Ѵ�:
						// - AddPlayerMappableConfig�� ������ ���� ���� ��õ
						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
					}
				}
				UCLMInputComponent* CLMIC = CastChecked<UCLMInputComponent>(PlayerInputComponent);
				CLMIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
				CLMIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
			}
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UCLMHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			// Left/Right -> X ���� �������:
			// MovementDirection�� ���� ī�޶��� RightVector�� �ǹ��� (World-Space)
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);

			// AddMovementInput �Լ��� �ѹ� ����:
			// - ���������� MovementDirection * Value.X�� MovementComponent�� ����(���ϱ�)���ش�
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f) // �ռ� �츮�� Forward ������ ���� swizzle input modifier�� ����ߴ�~
		{
			// �ռ� Left/Right�� ���������� Forward/Backward�� �����Ѵ�
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UCLMHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		// X���� Yaw ���� ����:
		// - Camera�� ���� Yaw ����
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		// Y���� Pitch ��!
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
}

TSubclassOf<UCLMCameraMode> UCLMHeroComponent::DetermineCameraMode() const
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UCLMPawnExtensionComponent* PawnExtComp = UCLMPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UCLMPawnData* PawnData = PawnExtComp->GetPawnData<UCLMPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}
