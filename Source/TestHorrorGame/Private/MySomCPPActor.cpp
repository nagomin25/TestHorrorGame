// Fill out your copyright notice in the Description page of Project Settings.


#include "MySomCPPActor.h"

// Sets default values
AMySomCPPActor::AMySomCPPActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMySomCPPActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMySomCPPActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

auto AMySomCPPActor::DelayOfSound(const float Distance) -> float
{
	static const float SoundVelocity = 340.29f;
	return Distance / SoundVelocity;
}

auto AMySomCPPActor::SetMyVelocity(const float Velocity) -> void
{
	MyVelocity = Velocity;
}

auto AMySomCPPActor::Delay(const float Distance) const -> float
{
	if (MyVelocity <= 0.0f)
	{
		return 0.0f;
	} else
	{
		return Distance / MyVelocity;
	}
}

