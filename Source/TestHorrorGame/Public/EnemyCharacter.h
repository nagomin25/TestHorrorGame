#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HorrorCharacterAnimInstance.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Patrolling	UMETA(DisplayName = "Patrolling"),
	Chasing		UMETA(DisplayName = "Chasing"),
	Attacking	UMETA(DisplayName = "Attacking"),
	Stunned		UMETA(DisplayName = "Stunned")
};

UCLASS()
class TESTHORRORGAME_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	virtual void Tick(float DeltaTime) override;

	// === 敵キャラクター設定 ===

	// 移動速度設定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	float ChaseSpeed;

	// 攻撃設定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	float AttackCooldown;

	// パトロール設定
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	TArray<FVector> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Settings")
	float PatrolWaitTime;

	// === 現在の状態 ===

	UPROPERTY(BlueprintReadOnly, Category = "Enemy State")
	EEnemyState CurrentState;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy State")
	AActor* TargetPlayer;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy State")
	bool bCanAttack;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy State")
	float LastAttackTime;

	// タイマーハンドル
	FTimerHandle AttackDamageTimerHandle;
	FTimerHandle AttackEndTimerHandle;

	// === 関数 ===

	// 状態変更
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SetEnemyState(EEnemyState NewState);

	// 攻撃関数
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void PerformAttack();

	// プレイヤーにダメージを与える
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void DealDamageToPlayer(AActor* Player);

	// 距離チェック
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	float GetDistanceToPlayer() const;

	// アニメーション関連
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	UHorrorCharacterAnimInstance* GetHorrorAnimInstance() const;

	// デバッグ用
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void DebugCurrentState() const;

protected:
	// 攻撃判定
	bool CanPerformAttack() const;

	// アニメーション更新
	void UpdateAnimationState();
};