#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemData.h"
#include "ItemActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class ATestHorrorGameCharacter;

UCLASS(Blueprintable)
class TESTHORRORGAME_API AItemActor : public AActor
{
	GENERATED_BODY()

public:
	AItemActor();

protected:
	virtual void BeginPlay() override;

public:
	// 見た目（Mesh）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	// 拾える範囲（当たり判定）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* PickupSphere;

	// 保持するアイテムデータ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FItemData ItemData;

	// インタラクト可能かどうか
	UPROPERTY(BlueprintReadOnly, Category="Item")
	bool bCanInteract;

	// インタラクト可能なプレイヤーキャラクター
	UPROPERTY()
	class ACharacter* PlayerInRange;

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

	// インタラクト関数
	UFUNCTION(BlueprintCallable, Category="Item")
	void Interact(ACharacter* Character);
};
