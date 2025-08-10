#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HorrorCharacterAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EHorrorMovementState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Walking		UMETA(DisplayName = "Walking"), 
	Jogging		UMETA(DisplayName = "Jogging"),
	Sneaking	UMETA(DisplayName = "Sneaking")
};

UENUM(BlueprintType)
enum class EHorrorActionState : uint8
{
	None		UMETA(DisplayName = "None"),
	Hiding		UMETA(DisplayName = "Hiding"),
	Interacting	UMETA(DisplayName = "Interacting"),
	Healing		UMETA(DisplayName = "Healing"),
	Grabbed		UMETA(DisplayName = "Grabbed"),
	Dead		UMETA(DisplayName = "Dead")
};

UCLASS(BlueprintType, Blueprintable)
class TESTHORRORGAME_API UHorrorCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UHorrorCharacterAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	// キャラクターリファレンス
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	ACharacter* OwningCharacter;

	// キャラクター移動コンポーネント
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	UCharacterMovementComponent* OwningMovementComponent;

public:
	// === 基本移動変数 ===
	
	// 移動速度
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed;

	// 移動方向（前後左右）
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Direction;

	// 空中にいるかどうか
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	// 移動しているかどうか
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsMoving;

	// === ホラーゲーム専用変数 ===

	// 現在の移動状態
	UPROPERTY(BlueprintReadOnly, Category = "Horror States")
	EHorrorMovementState MovementState;

	// 現在のアクション状態
	UPROPERTY(BlueprintReadOnly, Category = "Horror States")
	EHorrorActionState ActionState;

	// スニーク中かどうか
	UPROPERTY(BlueprintReadOnly, Category = "Horror States")
	bool bIsSneaking;

	// 隠れている最中かどうか
	UPROPERTY(BlueprintReadOnly, Category = "Horror States")
	bool bIsHiding;

	// 警戒状態かどうか（見回し）
	UPROPERTY(BlueprintReadOnly, Category = "Horror States")
	bool bIsLookingOut;

	// インタラクト中かどうか
	UPROPERTY(BlueprintReadOnly, Category = "Horror States")
	bool bIsInteracting;

	// 治療中かどうか
	UPROPERTY(BlueprintReadOnly, Category = "Horror States")
	bool bIsHealing;

	// 敵に捕まっているかどうか
	UPROPERTY(BlueprintReadOnly, Category = "Horror States")
	bool bIsGrabbed;

	// 死亡状態かどうか
	UPROPERTY(BlueprintReadOnly, Category = "Horror States")
	bool bIsDead;

	// === 速度しきい値設定 ===

	// 歩行判定の最小速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Thresholds")
	float WalkSpeedThreshold;

	// ジョギング判定の最小速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Thresholds")
	float JogSpeedThreshold;

	// === ブループリントから呼び出される関数 ===

	// アクション状態を設定
	UFUNCTION(BlueprintCallable, Category = "Horror Animation")
	void SetActionState(EHorrorActionState NewActionState);

	// スニーク状態を設定
	UFUNCTION(BlueprintCallable, Category = "Horror Animation")
	void SetSneaking(bool bNewSneaking);

	// 隠れ状態を設定
	UFUNCTION(BlueprintCallable, Category = "Horror Animation")
	void SetHiding(bool bNewHiding);

	// 警戒状態を設定
	UFUNCTION(BlueprintCallable, Category = "Horror Animation")
	void SetLookingOut(bool bNewLookingOut);

private:
	// 移動状態を更新
	void UpdateMovementState();

	// 各種フラグを更新
	void UpdateAnimationFlags();
};