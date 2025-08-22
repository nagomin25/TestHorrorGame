#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimEnums.h"
#include "Engine/Engine.h"
#include "Engine/DamageEvents.h"
#include "../TestHorrorGameCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// デフォルト設定
	WalkSpeed = 200.0f;
	ChaseSpeed = 450.0f;
	AttackRange = 150.0f; // 密着攻撃用に縮小
	AttackDamage = 20.0f;
	AttackCooldown = 3.0f; // クールダウンを3秒に延長
	PatrolWaitTime = 3.0f;

	// 初期状態
	CurrentState = EEnemyState::Idle;
	TargetPlayer = nullptr;
	bCanAttack = false;
	LastAttackTime = 0.0f;

	// Character Movement設定
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);
	
	// Root Motion無効化（位置ずれ防止）
	// UE5.5ではAnimBPで設定するため、ここではコメントアウト
	// GetMesh()->SetRootMotionMode(ERootMotionMode::IgnoreRootMotion);

	// コリジョン設定
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// メッシュの向きを修正（SurvivorBは90度回転が必要）
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	

	// AIControllerを設定
	AIControllerClass = AEnemyAIController::StaticClass();
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("👹 EnemyCharacter spawned: %s"), *GetName());
	
	// 初期パトロールポイント設定（レベルに配置されていない場合）
	if (PatrolPoints.Num() == 0)
	{
		FVector CurrentLocation = GetActorLocation();
		PatrolPoints.Add(CurrentLocation + FVector(500.0f, 0.0f, 0.0f));
		PatrolPoints.Add(CurrentLocation + FVector(0.0f, 500.0f, 0.0f));
		PatrolPoints.Add(CurrentLocation + FVector(-500.0f, 0.0f, 0.0f));
		PatrolPoints.Add(CurrentLocation + FVector(0.0f, -500.0f, 0.0f));
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 基本的なデバッグ情報（毎フレーム）
	static int32 TickCount = 0;
	if (++TickCount % 120 == 0) // 2秒おきに表示
	{
		UE_LOG(LogTemp, Warning, TEXT("🎯 EnemyCharacter Tick - TargetPlayer: %s, CurrentState: %s"), 
			TargetPlayer ? TEXT("FOUND") : TEXT("NONE"), 
			*UEnum::GetValueAsString(CurrentState));
	}

	// ターゲットプレイヤーの取得と状態更新
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
		TargetPlayer = AIController->GetTargetPlayer();
		
		// AI Controllerのblackboardから状態を同期
		if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
		{
			bool bIsChasing = BlackboardComp->GetValueAsBool(TEXT("IsChasing"));
			bool bCanAttackBB = BlackboardComp->GetValueAsBool(TEXT("CanAttack"));
			
			// 状態を更新
			if (bIsChasing && TargetPlayer)
			{
				if (CurrentState != EEnemyState::Chasing && CurrentState != EEnemyState::Attacking)
				{
					SetEnemyState(EEnemyState::Chasing);
				}
			}
			else if (!bIsChasing && CurrentState == EEnemyState::Chasing)
			{
				SetEnemyState(EEnemyState::Idle);
			}
			
			// デバッグログを減らす（必要時のみ表示）
			// UE_LOG(LogTemp, Warning, TEXT("🔄 State Sync - BB_IsChasing: %s, BB_CanAttack: %s, CurrentState: %s"), 
			//	bIsChasing ? TEXT("YES") : TEXT("NO"), 
			//	bCanAttackBB ? TEXT("YES") : TEXT("NO"), 
			//	*UEnum::GetValueAsString(CurrentState));
		}
	}

	// 攻撃可能かチェック
	bCanAttack = CanPerformAttack();
	
	// デバッグ情報を追加
	if (TargetPlayer && CurrentState == EEnemyState::Chasing)
	{
		float Distance = GetDistanceToPlayer();
		UE_LOG(LogTemp, Warning, TEXT("🔍 Debug - Distance: %.1f, bCanAttack: %s, CurrentState: %s"), 
			Distance, bCanAttack ? TEXT("YES") : TEXT("NO"), *UEnum::GetValueAsString(CurrentState));
	}
	
	// テスト用の簡単な攻撃コードを削除（正常に動作するため）
	// if (TargetPlayer)
	// {
	//     float Distance = GetDistanceToPlayer();
	//     if (Distance <= 500.0f)
	//     {
	//         UE_LOG(LogTemp, Warning, TEXT("🎯 SIMPLE ATTACK TEST - Distance: %.1f"), Distance);
	//         PerformAttack();
	//     }
	// }
	
	// 視界内にいる間は常にプレイヤーの位置を追跡（攻撃時以外）
	if (TargetPlayer && CurrentState != EEnemyState::Attacking)
	{
		// AIControllerのBlackboardを通じて最新のプレイヤー位置を更新
		if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
		{
			if (UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent())
			{
				// 現在の距離を計算
				float CurrentDistance = GetDistanceToPlayer();
				FVector PlayerCurrentLocation = TargetPlayer->GetActorLocation();
				
				// プレイヤーの位置を常に更新
				BlackboardComp->SetValueAsVector(TEXT("LastKnownLocation"), PlayerCurrentLocation);
				
				// 追跡状態なら直接移動指示を出す
				if (CurrentState == EEnemyState::Chasing)
				{
					AIController->MoveToLocation(PlayerCurrentLocation, 50.0f, true, true, true, true);
				}
				
				// 攻撃距離内かどうかを常に更新
				BlackboardComp->SetValueAsBool(TEXT("CanAttack"), CurrentDistance <= AttackRange);
				
				// 攻撃可能な場合のみ攻撃を実行
				if (bCanAttack && CurrentState == EEnemyState::Chasing)
				{
					UE_LOG(LogTemp, Warning, TEXT("🎯 About to perform attack! Distance: %.1f"), CurrentDistance);
					PerformAttack();
				}
			}
		}
	}

	// アニメーション状態を更新
	UpdateAnimationState();

	// デバッグ情報は無効化
}

void AEnemyCharacter::SetEnemyState(EEnemyState NewState)
{
	if (CurrentState == NewState) return;

	// オブジェクトの有効性を確認
	if (!IsValid(this) || !GetCharacterMovement())
	{
		return;
	}

	EEnemyState PreviousState = CurrentState;
	CurrentState = NewState;

	// 状態変更時の処理
	switch (NewState)
	{
		case EEnemyState::Idle:
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			break;

		case EEnemyState::Patrolling:
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			break;

		case EEnemyState::Chasing:
			GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
			break;

		case EEnemyState::Attacking:
			GetCharacterMovement()->MaxWalkSpeed = 0.0f; // 攻撃中は停止
			break;

		case EEnemyState::Stunned:
			GetCharacterMovement()->MaxWalkSpeed = 0.0f; // スタン中は停止
			break;
	}
}

void AEnemyCharacter::PerformAttack()
{
	if (!CanPerformAttack()) return;

	SetEnemyState(EEnemyState::Attacking);
	LastAttackTime = GetWorld()->GetTimeSeconds();

	UE_LOG(LogTemp, Warning, TEXT("⚔️ Enemy starting attack animation!"));

	// 攻撃アニメーションの途中でダメージを与える（タイマーで実装）
	FTimerHandle AttackDamageTimer;
	GetWorld()->GetTimerManager().SetTimer(AttackDamageTimer, [this]()
	{
		// オブジェクトの有効性確認
		if (!IsValid(this)) return;
		
		// アニメーション中にダメージを与える
		if (TargetPlayer && CurrentState == EEnemyState::Attacking)
		{
			DealDamageToPlayer(TargetPlayer);
		}
	}, 0.8f, false);

	// アニメーション実行後にチェイス状態に戻る（タイマーで実装）
	FTimerHandle AttackEndTimer;
	GetWorld()->GetTimerManager().SetTimer(AttackEndTimer, [this]()
	{
		// オブジェクトの有効性確認
		if (!IsValid(this)) return;
		
		SetEnemyState(EEnemyState::Chasing);
	}, 1.5f, false);
}

void AEnemyCharacter::DealDamageToPlayer(AActor* Player)
{
	if (!Player) return;

	// プレイヤーにダメージを与える処理
	if (ATestHorrorGameCharacter* HorrorPlayer = Cast<ATestHorrorGameCharacter>(Player))
	{
		UE_LOG(LogTemp, Error, TEXT("💥 Enemy dealing %.1f damage to player!"), AttackDamage);
		
		// プレイヤーのダメージ処理を実行 - ホラーゲームなので即死
		FDamageEvent DamageEvent;
		HorrorPlayer->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		
		// 視覚効果やサウンド再生もここで実装可能
		UE_LOG(LogTemp, Error, TEXT("🎯 Player has been attacked! Game should be over now."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ Player cast failed - cannot deal damage"));
	}
}

float AEnemyCharacter::GetDistanceToPlayer() const
{
	if (!TargetPlayer) return -1.0f;

	return FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());
}

UHorrorCharacterAnimInstance* AEnemyCharacter::GetHorrorAnimInstance() const
{
	return Cast<UHorrorCharacterAnimInstance>(GetMesh()->GetAnimInstance());
}

void AEnemyCharacter::DebugCurrentState() const
{
	UE_LOG(LogTemp, Warning, TEXT("👹 Enemy Debug - State: %s, Target: %s, Can Attack: %s"), 
		*UEnum::GetValueAsString(CurrentState),
		TargetPlayer ? *TargetPlayer->GetName() : TEXT("None"),
		bCanAttack ? TEXT("Yes") : TEXT("No"));
}

bool AEnemyCharacter::CanPerformAttack() const
{
	if (!TargetPlayer) 
	{
		return false;
	}
	if (CurrentState == EEnemyState::Attacking || CurrentState == EEnemyState::Stunned) 
	{
		return false;
	}
	
	float Distance = GetDistanceToPlayer();
	bool bInRange = Distance <= AttackRange && Distance > 0.0f;
	bool bCooldownReady = GetWorld()->GetTimeSeconds() - LastAttackTime >= AttackCooldown;

	// デバッグ情報を減らして、重要な情報のみ表示
	if (!bCooldownReady)
	{
		float RemainingCooldown = AttackCooldown - (GetWorld()->GetTimeSeconds() - LastAttackTime);
		UE_LOG(LogTemp, Warning, TEXT("⏰ Attack cooldown: %.1f seconds remaining"), RemainingCooldown);
	}

	return bInRange && bCooldownReady;
}

void AEnemyCharacter::UpdateAnimationState()
{
	UHorrorCharacterAnimInstance* AnimInstance = GetHorrorAnimInstance();
	if (!AnimInstance) 
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ AnimInstance is null in UpdateAnimationState"));
		return;
	}

	// 移動速度を取得
	FVector Velocity = GetVelocity();
	float CurrentSpeed = Velocity.Size();
	
	// 既存のプロパティに直接値を設定
	AnimInstance->Speed = CurrentSpeed;
	AnimInstance->bIsMoving = CurrentSpeed > 5.0f; // 5ユニット/秒以上で移動中（閾値を下げる）

	// デバッグログを追加 - アニメーション値の確認
	static int32 AnimDebugCount = 0;
	if (++AnimDebugCount % 60 == 0) // 1秒おきに表示
	{
		UE_LOG(LogTemp, Warning, TEXT("🎬 Animation Debug - Speed: %.2f, bIsMoving: %s, CurrentState: %s"), 
			CurrentSpeed, 
			AnimInstance->bIsMoving ? TEXT("TRUE") : TEXT("FALSE"),
			*UEnum::GetValueAsString(CurrentState));
	}

	// 敵キャラクターのアクション状態を設定
	switch (CurrentState)
	{
		case EEnemyState::Idle:
			// アイドル状態 - 移動速度で自動判定される
			AnimInstance->SetActionState(EHorrorActionState::None);
			break;

		case EEnemyState::Patrolling:
		case EEnemyState::Chasing:
			// 移動アニメーション - 移動速度で自動判定される
			AnimInstance->SetActionState(EHorrorActionState::None);
			break;

		case EEnemyState::Attacking:
			// 攻撃アニメーション
			AnimInstance->SetActionState(EHorrorActionState::Interacting);
			break;

		case EEnemyState::Stunned:
			// スタンアニメーション
			AnimInstance->SetActionState(EHorrorActionState::Grabbed);
			break;
	}
}