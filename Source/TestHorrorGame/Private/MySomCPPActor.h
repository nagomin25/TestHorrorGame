// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySomCPPActor.generated.h"

UCLASS()
class AMySomCPPActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySomCPPActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float MyVelocity;

public:
	UFUNCTION(BlueprintCallable, Category = "UE C++ Book")
	static float DelayOfSound(const float Distance);
	
	UFUNCTION(BlueprintCallable, Category = "UE C++ Book")
	void SetMyVelocity(const float Velocity);

	UFUNCTION(BlueprintPure, Category = "UE C++ Book")
	float Delay(const float Distance) const;

};
