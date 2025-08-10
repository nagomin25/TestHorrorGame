#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "GameFramework/Character.h"
#include "Engine/Engine.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "../TestHorrorGameCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// Blackboard Component (inherited from AIController)
	// BehaviorTreeComponent (inherited from AIController)

	// AI Perception Component
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// Sight Configuration - より賢い追跡設定
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 2000.0f;          // 視界距離を拡大
		SightConfig->LoseSightRadius = 2200.0f;      // 見失う距離も拡大
		SightConfig->PeripheralVisionAngleDegrees = 120.0f; // 視野角を拡大
		SightConfig->SetMaxAge(10.0f);               // 記憶時間を大幅延長
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 800.0f; // 最後に見た位置周辺での自動検出を拡大
		
		// Player detection
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;

		AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
		AIPerceptionComponent->ConfigureSense(*SightConfig);
	}

	// Set up perception events
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);

	// Initialize Blackboard Keys
	TargetPlayerKey = TEXT("TargetPlayer");
	LastKnownLocationKey = TEXT("LastKnownLocation");
	IsChasing = TEXT("IsChasing");
	CanAttack = TEXT("CanAttack");

	// Set default assets
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAsset(TEXT("/Game/BT_Enemy"));
	if (BTAsset.Succeeded())
	{
		BehaviorTreeAsset = BTAsset.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAsset(TEXT("/Game/BB_Enemy"));
	if (BBAsset.Succeeded())
	{
		BlackboardAsset = BBAsset.Object;
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// Start Behavior Tree
	if (BlackboardAsset && BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ EnemyAIController: BehaviorTreeAsset or BlackboardAsset not set!"));
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Initialize Blackboard
	if (BlackboardAsset)
	{
		// Use the blackboard
		if (Blackboard)
		{
			Blackboard->InitializeBlackboard(*BlackboardAsset);
		}
		
		// Initialize blackboard values
		if (GetBlackboardComponent())
		{
			GetBlackboardComponent()->SetValueAsBool(IsChasing, false);
			GetBlackboardComponent()->SetValueAsBool(CanAttack, false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ EnemyAIController: BlackboardAsset not set!"));
	}

	UE_LOG(LogTemp, Warning, TEXT("🤖 EnemyAIController possessed pawn: %s"), *InPawn->GetName());
}

void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		// プレイヤーキャラクターを検出
		if (ATestHorrorGameCharacter* Player = Cast<ATestHorrorGameCharacter>(Actor))
		{
			FActorPerceptionBlueprintInfo Info;
			AIPerceptionComponent->GetActorsPerception(Actor, Info);

			bool bPlayerCurrentlyVisible = false;
			for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
			{
				if (Stimulus.WasSuccessfullySensed())
				{
					bPlayerCurrentlyVisible = true;
					
					// プレイヤーが視界内にいる場合
					if (!GetBlackboardComponent()->GetValueAsBool(IsChasing))
					{
						UE_LOG(LogTemp, Warning, TEXT("👁️ Enemy detected player: %s"), *Player->GetName());
					}
					
					StartChasing(Player);
					
					// プレイヤーの現在位置を毎フレーム更新（視界内にいる限り）
					FVector PlayerCurrentLocation = Player->GetActorLocation();
					GetBlackboardComponent()->SetValueAsVector(LastKnownLocationKey, PlayerCurrentLocation);
					
					// リアルタイムで直接プレイヤーに向かって移動
					MoveToLocation(PlayerCurrentLocation, 50.0f, true, true, true, true);
					
					// 攻撃距離の更新（密着攻撃用に縮小）
					float DistanceToTarget = FVector::Dist(GetPawn()->GetActorLocation(), PlayerCurrentLocation);
					GetBlackboardComponent()->SetValueAsBool(CanAttack, DistanceToTarget <= 150.0f);
					
					// デバッグログ - より詳細な追跡情報
					static int32 TrackingLogCount = 0;
					if (++TrackingLogCount % 60 == 0) // 1秒おきに表示
					{
						UE_LOG(LogTemp, Warning, TEXT("🎯 Continuously tracking player - Distance: %.1f, Moving to: %s"), 
							DistanceToTarget, *PlayerCurrentLocation.ToString());
					}
					
					break; // プレイヤーを発見したらループを抜ける
				}
			}
			
			if (!bPlayerCurrentlyVisible)
			{
				// プレイヤーを見失った場合 - ただし記憶時間内は追跡継続
				UE_LOG(LogTemp, Warning, TEXT("👁️ Enemy lost sight of player - but continuing pursuit to last known location"));
				
				// 見失ってもすぐには追跡を停止しない（記憶時間に依存）
				// Blackboardの値は維持し、Behavior Treeで最後の位置への移動を継続
			}
		}
	}
}

void AEnemyAIController::StartChasing(AActor* Target)
{
	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject(TargetPlayerKey, Target);
		GetBlackboardComponent()->SetValueAsBool(IsChasing, true);
		GetBlackboardComponent()->SetValueAsVector(LastKnownLocationKey, Target->GetActorLocation());

		// 攻撃距離内かチェック（密着攻撃用に縮小）
		float DistanceToTarget = FVector::Dist(GetPawn()->GetActorLocation(), Target->GetActorLocation());
		GetBlackboardComponent()->SetValueAsBool(CanAttack, DistanceToTarget <= 150.0f);

		UE_LOG(LogTemp, Warning, TEXT("🏃‍♂️ Enemy started chasing: %s (Distance: %.1f) CanAttack: %s"), 
			*Target->GetName(), DistanceToTarget, DistanceToTarget <= 150.0f ? TEXT("YES") : TEXT("NO"));
	}
}

void AEnemyAIController::StopChasing()
{
	if (GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject(TargetPlayerKey, nullptr);
		GetBlackboardComponent()->SetValueAsBool(IsChasing, false);
		GetBlackboardComponent()->SetValueAsBool(CanAttack, false);

		UE_LOG(LogTemp, Warning, TEXT("⏹️ Enemy stopped chasing"));
	}
}

AActor* AEnemyAIController::GetTargetPlayer() const
{
	if (GetBlackboardComponent())
	{
		return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TargetPlayerKey));
	}
	return nullptr;
}