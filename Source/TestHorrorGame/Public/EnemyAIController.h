#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyAIController.generated.h"

UCLASS()
class TESTHORRORGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	// Behavior Tree Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	// Blackboard Asset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBlackboardData* BlackboardAsset;

	// AI Perception Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	// Sight Configuration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

public:
	// Blackboard Keys
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName TargetPlayerKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName LastKnownLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName IsChasing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName CanAttack;

	// AI Perception Events
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	// Utility Functions
	UFUNCTION(BlueprintCallable, Category = "AI")
	void StartChasing(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void StopChasing();

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetPlayer() const;
};