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

	// 올바른 Actor에 등록되었는지 확인:
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

	// PawnExtensionComponent에 대해서 (PawnExtension Feature) OnActorInitStateChanged() 관찰하도록 (Observing)
	BindOnActorInitStateChanged(UCLMPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// InitState_Spawned로 초기화
	ensure(TryToChangeInitState(FCLMGameplayTags::Get().InitState_Spawned));

	// ForceUpdate 진행
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
		// CLMPawnExtensionComponent의 DataInitialized 상태 변화 관찰 후, CLMHeroComponent도 DataInitialized 상태로 변경
		// - CanChangeInitState 확인
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

	// Spawned 초기화
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
		// PawnExtensionComponent가 DataInitialized될 때까지 기다림 (== 모든 Feature Component가 DataAvailable인 상태)
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
	// 앞서 BindOnActorInitStateChanged에서 보았듯이 Hero Feature는 Pawn Extension Feature에 종속되어 있으므로, CheckDefaultInitializationForImplementers 호출하지 않음:

	// ContinueInitStateChain은 앞서 PawnExtComponent와 같음
	const FCLMGameplayTags& InitTags = FCLMGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	auto CurrentState = ContinueInitStateChain(StateChain);
	UE_LOG(LogCLM, Log, TEXT("UCLMHeroComponent::CheckDefaultInitialization: %s"), *CurrentState.ToString());
}

void UCLMHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FCLMGameplayTags& InitTags = FCLMGameplayTags::Get();

	// DataAvailable -> DataInitialized 단계
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

			// DataInitialized 단계까지 오면, Pawn이 Controller에 Possess되어 준비된 상태이다:
			// - InitAbilityActorInfo 호출로 AvatarActor 재설정이 필요하다
			//PawnExtComp->InitializeAbilitySystem(CLMPS->GetCLMAbilitySystemComponent(), CLMPS);
		}

		// Input 핸들링
		if (ACLMPlayerController* CLMPC = GetController<ACLMPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		if (PawnData)
		{
			// Camera 핸들링
			// 현재 CLMCharacter에 Attach된 CameraComponent를 찾음
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

	// LocalPlayer를 가져오기 위해
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	// EnhancedInputLocalPlayerSubsystem 가져오기 위해
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// EnhancedInputLocalPlayerSubsystem에 MappingContext를 비워준다:
	Subsystem->ClearAllMappings();

	// PawnExtensionComponent -> PawnData -> InputConfig 존재 유무 판단:
	if (const UCLMPawnExtensionComponent* PawnExtComp = UCLMPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UCLMPawnData* PawnData = PawnExtComp->GetPawnData<UCLMPawnData>())
		{
			if (const UCLMInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FCLMGameplayTags& GameplayTags = FCLMGameplayTags::Get();

				// HeroComponent 가지고 있는 Input Mapping Context를 순회하며, EnhancedInputLocalPlayerSubsystem에 추가한다
				for (const FCLMMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						// 내부적으로 Input Mapping Context를 추가한다:
						// - AddPlayerMappableConfig를 간단히 보는 것을 추천
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
			// Left/Right -> X 값에 들어있음:
			// MovementDirection은 현재 카메라의 RightVector를 의미함 (World-Space)
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);

			// AddMovementInput 함수를 한번 보자:
			// - 내부적으로 MovementDirection * Value.X를 MovementComponent에 적용(더하기)해준다
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f) // 앞서 우리는 Forward 적용을 위해 swizzle input modifier를 사용했다~
		{
			// 앞서 Left/Right와 마찬가지로 Forward/Backward를 적용한다
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
		// X에는 Yaw 값이 있음:
		// - Camera에 대해 Yaw 적용
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		// Y에는 Pitch 값!
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
