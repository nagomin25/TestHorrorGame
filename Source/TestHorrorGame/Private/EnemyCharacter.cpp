#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/Engine.h"
#include "../TestHorrorGameCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// デフォルト設定
	WalkSpeed = 200.0f;
	ChaseSpeed = 450.0f;
	AttackRange = 150.0f;
	AttackDamage = 20.0f;
	AttackCooldown = 2.0f;
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

	// コリジョン設定
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	// メッシュの向きを修正（SurvivorBは90度回転が必要）
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	// Arrow Componentの向きも修正
	if (GetArrowComponent())
	{
		GetArrowComponent()->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}

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

	// ターゲットプレイヤーの取得
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
		TargetPlayer = AIController->GetTargetPlayer();
	}

	// 攻撃可能かチェック
	bCanAttack = CanPerformAttack();

	// アニメーション状態を更新
	UpdateAnimationState();

	// デバッグ情報（開発用）
	if (GEngine && CurrentState == EEnemyState::Chasing && TargetPlayer)
	{
		float Distance = GetDistanceToPlayer();
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, 
			FString::Printf(TEXT("Enemy: %s | Distance: %.1f | Can Attack: %s"), 
				*UEnum::GetValueAsString(CurrentState), 
				Distance, 
				bCanAttack ? TEXT("Yes") : TEXT("No")
			));
	}
}

void AEnemyCharacter::SetEnemyState(EEnemyState NewState)
{
	if (CurrentState == NewState) return;

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

	UE_LOG(LogTemp, Warning, TEXT("👹 Enemy state changed: %s → %s"), 
		*UEnum::GetValueAsString(PreviousState), 
		*UEnum::GetValueAsString(NewState));
}

void AEnemyCharacter::PerformAttack()
{
	if (!CanPerformAttack()) return;

	SetEnemyState(EEnemyState::Attacking);
	LastAttackTime = GetWorld()->GetTimeSeconds();

	UE_LOG(LogTemp, Warning, TEXT("⚔️ Enemy performing attack!"));

	// プレイヤーにダメージを与える
	if (TargetPlayer)
	{
		DealDamageToPlayer(TargetPlayer);
	}

	// アニメーション実行後にアイドル状態に戻る（タイマーで実装）
	FTimerHandle AttackTimer;
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, [this]()
	{
		SetEnemyState(EEnemyState::Chasing);
	}, 1.5f, false);
}

void AEnemyCharacter::DealDamageToPlayer(AActor* Player)
{
	if (!Player) return;

	// プレイヤーにダメージを与える処理
	if (ATestHorrorGameCharacter* HorrorPlayer = Cast<ATestHorrorGameCharacter>(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("💥 Enemy dealt %.1f damage to player!"), AttackDamage);
		
		// 実際のダメージシステムを実装する場合はここに追加
		// 例：HorrorPlayer->TakeDamage(AttackDamage);
		
		// 視覚効果やサウンド再生もここで実装可能
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
	if (!TargetPlayer) return false;
	if (CurrentState == EEnemyState::Attacking || CurrentState == EEnemyState::Stunned) return false;
	
	float Distance = GetDistanceToPlayer();
	bool bInRange = Distance <= AttackRange && Distance > 0.0f;
	bool bCooldownReady = GetWorld()->GetTimeSeconds() - LastAttackTime >= AttackCooldown;

	return bInRange && bCooldownReady;
}

void AEnemyCharacter::UpdateAnimationState()
{
	UHorrorCharacterAnimInstance* AnimInstance = GetHorrorAnimInstance();
	if (!AnimInstance) return;

	// 敵キャラクターのアニメーション状態を設定
	switch (CurrentState)
	{
		case EEnemyState::Idle:
			// アイドルアニメーション
			break;

		case EEnemyState::Patrolling:
		case EEnemyState::Chasing:
			// 移動アニメーション
			break;

		case EEnemyState::Attacking:
			// 攻撃アニメーション
			AnimInstance->SetActionState(EHorrorActionState::Interacting); // 一時的に使用
			break;

		case EEnemyState::Stunned:
			// スタンアニメーション
			AnimInstance->SetActionState(EHorrorActionState::Grabbed); // 一時的に使用
			break;
	}
}