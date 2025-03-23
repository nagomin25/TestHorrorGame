// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstRiderActor.h"

// Sets default values
AFirstRiderActor::AFirstRiderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFirstRiderActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Hello Rider World!"));
	
}

// Called every frame
void AFirstRiderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

