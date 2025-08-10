#include "HorrorCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UHorrorCharacterAnimInstance::UHorrorCharacterAnimInstance()
{
	// デフォルト値を設定
	Speed = 0.0f;
	Direction = 0.0f;
	bIsInAir = false;
	bIsMoving = false;
	
	MovementState = EHorrorMovementState::Idle;
	ActionState = EHorrorActionState::None;
	
	bIsSneaking = false;
	bIsHiding = false;
	bIsLookingOut = false;
	bIsInteracting = false;
	bIsHealing = false;
	bIsGrabbed = false;
	bIsDead = false;
	
	// 速度しきい値のデフォルト値
	WalkSpeedThreshold = 10.0f;
	JogSpeedThreshold = 300.0f;
	
	OwningCharacter = nullptr;
	OwningMovementComponent = nullptr;
}

void UHorrorCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	// キャラクターリファレンスを取得
	OwningCharacter = Cast<ACharacter>(GetOwningActor());
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UHorrorCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}
	
	// 基本的な移動データを更新
	FVector Velocity = OwningMovementComponent->Velocity;
	Speed = Velocity.Size2D();
	bIsInAir = OwningMovementComponent->IsFalling();
	bIsMoving = Speed > 3.0f;
	
	// 移動方向を計算（キャラクターの向きに対する相対的な方向）
	if (bIsMoving)
	{
		FVector Forward = OwningCharacter->GetActorForwardVector();
		FVector VelocityNormalized = Velocity.GetSafeNormal2D();
		Direction = FVector::DotProduct(Forward, VelocityNormalized);
	}
	else
	{
		Direction = 0.0f;
	}
	
	// 移動状態を更新
	UpdateMovementState();
	
	// アニメーションフラグを更新
	UpdateAnimationFlags();
}

void UHorrorCharacterAnimInstance::UpdateMovementState()
{
	// デッド状態の場合は他の状態に変更しない
	if (bIsDead)
	{
		MovementState = EHorrorMovementState::Idle;
		return;
	}
	
	// 空中にいる場合はアイドル扱い
	if (bIsInAir)
	{
		MovementState = EHorrorMovementState::Idle;
		return;
	}
	
	// スニーク中の場合
	if (bIsSneaking && bIsMoving)
	{
		MovementState = EHorrorMovementState::Sneaking;
		return;
	}
	
	// 速度に基づいて移動状態を決定
	if (Speed < WalkSpeedThreshold)
	{
		MovementState = EHorrorMovementState::Idle;
	}
	else if (Speed < JogSpeedThreshold)
	{
		MovementState = EHorrorMovementState::Walking;
	}
	else
	{
		MovementState = EHorrorMovementState::Jogging;
	}
}

void UHorrorCharacterAnimInstance::UpdateAnimationFlags()
{
	// アクション状態に基づいてフラグを更新
	switch (ActionState)
	{
		case EHorrorActionState::None:
			bIsInteracting = false;
			bIsHealing = false;
			bIsGrabbed = false;
			break;
			
		case EHorrorActionState::Hiding:
			bIsHiding = true;
			break;
			
		case EHorrorActionState::Interacting:
			bIsInteracting = true;
			break;
			
		case EHorrorActionState::Healing:
			bIsHealing = true;
			break;
			
		case EHorrorActionState::Grabbed:
			bIsGrabbed = true;
			break;
			
		case EHorrorActionState::Dead:
			bIsDead = true;
			break;
	}
}

void UHorrorCharacterAnimInstance::SetActionState(EHorrorActionState NewActionState)
{
	ActionState = NewActionState;
}

void UHorrorCharacterAnimInstance::SetSneaking(bool bNewSneaking)
{
	bIsSneaking = bNewSneaking;
}

void UHorrorCharacterAnimInstance::SetHiding(bool bNewHiding)
{
	bIsHiding = bNewHiding;
	
	// 隠れ状態に応じてアクション状態も更新
	if (bNewHiding)
	{
		ActionState = EHorrorActionState::Hiding;
	}
	else if (ActionState == EHorrorActionState::Hiding)
	{
		ActionState = EHorrorActionState::None;
	}
}

void UHorrorCharacterAnimInstance::SetLookingOut(bool bNewLookingOut)
{
	bIsLookingOut = bNewLookingOut;
}