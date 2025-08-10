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

	// Sight Configuration
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 1200.0f;
		SightConfig->LoseSightRadius = 1300.0f;
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 300.0f;
		
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

			for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
			{
				if (Stimulus.WasSuccessfullySensed())
				{
					UE_LOG(LogTemp, Warning, TEXT("👁️ Enemy detected player: %s"), *Player->GetName());
					StartChasing(Player);
					
					// プレイヤーの位置を更新
					GetBlackboardComponent()->SetValueAsVector(LastKnownLocationKey, Player->GetActorLocation());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("👁️ Enemy lost sight of player"));
					// プレイヤーを見失った場合の処理
					GetBlackboardComponent()->SetValueAsObject(TargetPlayerKey, nullptr);
					GetBlackboardComponent()->SetValueAsBool(IsChasing, false);
				}
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

		// 攻撃距離内かチェック
		float DistanceToTarget = FVector::Dist(GetPawn()->GetActorLocation(), Target->GetActorLocation());
		GetBlackboardComponent()->SetValueAsBool(CanAttack, DistanceToTarget <= 300.0f); // テスト用に拡大

		UE_LOG(LogTemp, Warning, TEXT("🏃‍♂️ Enemy started chasing: %s (Distance: %.1f) CanAttack: %s"), 
			*Target->GetName(), DistanceToTarget, DistanceToTarget <= 300.0f ? TEXT("YES") : TEXT("NO"));
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