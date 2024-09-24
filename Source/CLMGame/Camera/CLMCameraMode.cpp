#include "CLMCameraMode.h"
#include "CLMPlayerCameraManager.h"
#include "CLMCameraComponent.h"

#pragma region CameraModeView

FCLMCameraModeView::FCLMCameraModeView()
	: Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(CLM_CAMERA_DEFAULT_FOV)
{
}

void FCLMCameraModeView::Blend(const FCLMCameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.0f)
	{
		return;
	}
	else if (OtherWeight >= 1.0f)
	{
		// Weight�� 1.0�̸� Other�� ����� �ȴ�
		*this = Other;
		return;
	}

	// Location + OtherWeight * (Other.Location - Location);
	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	// Location�� ���� ��� Lerp (ControlRotation�� FieldOfView�� ����)
	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

#pragma endregion


#pragma region CameraMode

UCLMCameraMode::UCLMCameraMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	FieldOfView = CLM_CAMERA_DEFAULT_FOV;
	ViewPitchMin = CLM_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = CLM_CAMERA_DEFAULT_PITCH_MAX;

	BlendTime = 0.5f;
	BlendFunction = ECLMCameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;
}

void UCLMCameraMode::UpdateCameraMode(float DeltaTime)
{
	// Actor�� Ȱ���Ͽ�, Pivot[Location|Rotation]�� ����Ͽ�, View�� ������Ʈ
	UpdateView(DeltaTime);

	// BlendWeight�� DeltaTime�� Ȱ���Ͽ�, BlendAlpha ��� ��, BlendFunction�� �°� ��-�����Ͽ� ���� ���
	UpdateBlending(DeltaTime);
}

void UCLMCameraMode::UpdateView(float DeltaTime)
{
	// CameraMode�� ������ �ִ� CameraComponent�� Owner�� Character(Pawn)�� Ȱ���Ͽ�, PivotLocation/Rotation�� ��ȯ��
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	// Pitch ���� ���� Min/Max�� Clamp��Ŵ
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	// FCLMCameraModeView�� PivotLocation/Rotation ����
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;

	// PivotRotation�� �Ȱ��� ControlRotation���� Ȱ��
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// �����ϸ�, Character�� Location�� ControlRotation�� Ȱ���Ͽ�, View�� ������Ʈ��
}

void UCLMCameraMode::UpdateBlending(float DeltaTime)
{
	// BlendAlpha�� DeltaTime�� ���� ���
	if (BlendTime > 0.f)
	{
		// BlendTime�� Blending ���� �� �ð�(��)
		// - BlendAlpha�� 0 -> 1�� ��ȭ�ϴ� ����:
		// - DeltaTime�� Ȱ���Ͽ�, BlendTime�� 1�� �� ���, ���� ������ ����
		BlendAlpha += (DeltaTime / BlendTime);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	// BlendWeight�� [0,1]�� BlendFunction�� �°� �����
	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;
	switch (BlendFunction)
	{
	case ECLMCameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;
	case ECLMCameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case ECLMCameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	case ECLMCameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;
	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

UCLMCameraComponent* UCLMCameraMode::GetCLMCameraComponent() const
{
	// �츮�� �ռ� UCLMCameraMode�� �����ϴ� ���� UCLMCameraModeStack�̾���:
	// - �ش� �ڵ带 ����, GetOuter()�� CLMCameraMode�� CLMCameraComponent�� �����Ͽ���
	// - UCLMCameraModeStack::GetCameraModeInstance() Ȯ��
	return CastChecked<UCLMCameraComponent>(GetOuter());
}

AActor* UCLMCameraMode::GetTargetActor() const
{
	const UCLMCameraComponent* CLMCameraComponent = GetCLMCameraComponent();
	return CLMCameraComponent->GetTargetActor();
}

FVector UCLMCameraMode::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// BaseEyeHeight�� ����Ͽ�, ViewLocation�� ��ȯ��
		return TargetPawn->GetPawnViewLocation();
	}

	return TargetActor->GetActorLocation();
}

FRotator UCLMCameraMode::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor))
	{
		// GetViewRoation() Ȯ��, ���� Pawn�� ControlRotation�� ��ȯ
		return TargetPawn->GetViewRotation();
	}

	return TargetActor->GetActorRotation();
}
#pragma endregion

#pragma region CLMCameraModeStack
UCLMCameraModeStack::UCLMCameraModeStack(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

UCLMCameraMode* UCLMCameraModeStack::GetCameraModeInstance(TSubclassOf<UCLMCameraMode>& CameraModeClass)
{
	check(CameraModeClass);

	for (UCLMCameraMode* CameraMode : CameraModeInstances)
	{
		if ((CameraMode != nullptr) && (CameraMode->GetClass() == CameraModeClass))
		{
			return CameraMode;
		}
	}

	// CameraModeClass �� �˸´� CameraMode�� �ν��Ͻ��� ���ٸ� ����
	UCLMCameraMode* NewCameraMode = NewObject<UCLMCameraMode>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(NewCameraMode);

	CameraModeInstances.Add(NewCameraMode);
	return NewCameraMode;
}

void UCLMCameraModeStack::PushCameraMode(TSubclassOf<UCLMCameraMode>& CameraModeClass)
{
	if (!CameraModeClass)
	{
		return;
	}

	UCLMCameraMode* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();
	if ((StackSize > 0) && (CameraModeStack[0] == CameraMode))
	{
		// CameraModeStack[0] ���� �ֱٿ� �̹� CameraMode�� Stacking�Ǿ����Ƿ� �׳� ����!
		return;
	}

	// ExistingStackIndex�� CameraModeStack���� CameraMode�� �´� Index�� ã��
	// ExistingStackContribution�� ������ �Ʒ��� ���� BlendWeight ���� ã�� ���� �ʱⰪ���� 1.0���� ����
	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;

	/// ī�޶� ��ü �Ǹ鼭 �ð��� �帧���� ������ ������ ���ִ� ������ �ϴ°� ����
	/// 3��Ī ī�޶� ���� �ڵ��� �������� ����� �ڿ������� ���� ���ִ� ��� ����
	/**
	 * BlendWeight    |    ExistingStackCOntribution    |    ExistingStackCOntribution (accumulated)
	 * 0.1f           |    (1.0f) * 0.1f = 0.1f         |    (1.0f - 0.1f) = 0.9f
	 * 0.3f           |    (0.9f) * 0.3f = 0.27f        |    (1.0f - 0.3f) * 0.9f = 0.63f
	 * 0.6f           |    (0.63f) * 0.6f = 0.378f      |    (1.0f - 0.6f) * 0.63f = 0.252f
	 * 1.0f           |    (0.252f) * 1.0f = 0.252f     |
	 *                |    0.1f + 0.27f + 0.378f + 0.252f = 1.0f!
	 */
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			// BlendWeight�� CameraMode�� �߰�������:
			// - ���⼭ ExistingStackContribution �츮�� ã�� CameraMode�� ã�����ϱ�, ������ BlendWeight�� �Բ� BlendWeight�� ���Ͽ�, ������ �������´�
			ExistingStackContribution *= CameraMode->BlendWeight;
			break;
		}
		else
		{
			// �翬�� �츮�� ���ϴ� CamearMode�� �ƴϴϱ�, InvBlendWeight = (1.0 - BlendWeight)�� �������, ���� ����?
			ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->BlendWeight);
		}
	}
	// �ش� ������ ���� ������ �ռ� ������� ǥ�� Ȯ���غ��� �����غ���.

	// �츮�� CameraMode�� Top���� �ݿ��ϱ� ����, �翬�� �߰��� �־��ٸ�, �����Ͽ� �ٽ� Push ����� �Ѵ�
	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		// �������ϱ� �翬�� Contribution�� 0���� ��������߰�¡?
		ExistingStackContribution = 0.0f;
	}

	// BlendTime�� 0���� ũ�ٴ� ���� Blend�� �� �ð����� �������� �ǹ� ����, ExistingStackContribution�� ����
	// - ���� Blend���� �ʴ´ٸ�, BlendWeight�� 1.0�� �־� ���� �ִ� CameraMode�� ������ ���̴�
	const bool bShouldBlend = ((CameraMode->BlendTime > 0.f) && (StackSize > 0));
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);
	CameraMode->BlendWeight = BlendWeight;

	// ��.. Array�� Stackó�� ����ϴ°��� ������, Index 0�� �ִ°� ��ȿ�����ε�..
	// - Push, Pop �޼���� ���̱׳� last�� �ִ°�... ��� ����
	CameraModeStack.Insert(CameraMode, 0);
	
	// �ռ� �����ߵ��� �������� �׻� 1.0�� �Ǿ�� ��!
	CameraModeStack.Last()->BlendWeight = 1.0f;
}

void UCLMCameraModeStack::EvaluateStack(float DeltaTime, FCLMCameraModeView& OutCameraModeView)
{
	// Top -> Bottom [0 -> Num]���� ���������� Stack�� �ִ� CameraMode ������Ʈ
	UpdateStack(DeltaTime);

	// Bottom -> Top���� CameraModeStack�� ���� Blending ����
	BlendStack(OutCameraModeView);
}

void UCLMCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// CameraModeStack�� ��ȸ�ϸ�, CameraMode�� ������Ʈ�Ѵ�
	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;
	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		UCLMCameraMode* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		// UpdateCameraMode�� Ȯ���غ���:
		CameraMode->UpdateCameraMode(DeltaTime);

		// ���� �ϳ��� CameraMode�� BlendWeight�� 1.0�� �����ߴٸ�, �� ���� CameraMode�� �����Ѵ�
		if (CameraMode->BlendWeight >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		// �����غ��� �̰� ������ Pop�ϰ� Push�� ���Ѱ�������?
		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void UCLMCameraModeStack::BlendStack(FCLMCameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0)
	{
		return;
	}

	// CameraModeStack�� Bottom -> Top ������ Blend�� �����Ѵ�
	const UCLMCameraMode* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);

	// ����� ���� �Ʒ� (Bottom)�� BlendWeight�� 1.0�̴�!
	OutCameraModeView = CameraMode->View;

	// �̹� Index = [StackSize - 1] �̹� OutCameraModeView�� ���������Ƿ�, StackSize - 2���� ��ȸ�ϸ� �ȴ�
	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		// CLMCameraModeView Blend �Լ��� ��������:
		OutCameraModeView.Blend(CameraMode->View, CameraMode->BlendWeight);
	}
}
#pragma endregion