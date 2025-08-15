#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DoorActor.generated.h"

UCLASS()
class TESTHORRORGAME_API ADoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADoorActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* InteractionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings")
	FName RequiredKeyID;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	bool bCanInteract;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	class ATestHorrorGameCharacter* PlayerInRange;

public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
						AActor* OtherActor,
						UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
					  AActor* OtherActor,
					  UPrimitiveComponent* OtherComp,
					  int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Interact(class ACharacter* Character);

private:
	UFUNCTION()
	void GameClear();
};