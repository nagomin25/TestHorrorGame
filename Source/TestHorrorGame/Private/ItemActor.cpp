#include "ItemActor.h"
#include "TestHorrorGame/TestHorrorGameCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"



AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	PickupSphere->SetupAttachment(RootComponent);
	PickupSphere->SetSphereRadius(250.f);  // 机の上のアイテム用に大きめに
	PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PickupSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);  // 机などの静的メッシュを無視

	bCanInteract = false;
	PlayerInRange = nullptr;
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlapBegin);
	PickupSphere->OnComponentEndOverlap.AddDynamic(this, &AItemActor::OnOverlapEnd);
	
	// デバッグ用：コリジョン球を可視化（開発中のみ）
	#if WITH_EDITOR
	PickupSphere->SetHiddenInGame(false);
	PickupSphere->SetVisibility(true);
	UE_LOG(LogTemp, Warning, TEXT("🔍 アイテム「%s」のPickupSphere半径: %.1f"), *ItemData.Name.ToString(), PickupSphere->GetScaledSphereRadius());
	#endif
}

void AItemActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
								 AActor* OtherActor,
								 UPrimitiveComponent* OtherComp,
								 int32 OtherBodyIndex,
								 bool bFromSweep,
								 const FHitResult& SweepResult)
{
	ATestHorrorGameCharacter* Character = Cast<ATestHorrorGameCharacter>(OtherActor);
	if (Character && Character->IsPlayerControlled())
	{
		bCanInteract = true;
		PlayerInRange = Character;
		
		// キャラクターのインタラクトリストに追加
		Character->InteractableItems.AddUnique(this);
		
		// デバッグ情報を追加
		float Distance = FVector::Dist(GetActorLocation(), Character->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("🔑 アイテム「%s」がインタラクト可能になりました (距離: %.1f)"), *ItemData.Name.ToString(), Distance);
		UE_LOG(LogTemp, Warning, TEXT("🎯 アイテム位置: %s, プレイヤー位置: %s"), 
			*GetActorLocation().ToString(), *Character->GetActorLocation().ToString());
	}
}

void AItemActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
							   AActor* OtherActor,
							   UPrimitiveComponent* OtherComp,
							   int32 OtherBodyIndex)
{
	ATestHorrorGameCharacter* Character = Cast<ATestHorrorGameCharacter>(OtherActor);
	if (Character && Character == PlayerInRange)
	{
		bCanInteract = false;
		PlayerInRange = nullptr;
		
		// キャラクターのインタラクトリストから削除
		Character->InteractableItems.Remove(this);
		
		// UIのインタラクト表示を非表示に
		UE_LOG(LogTemp, Warning, TEXT("アイテムがインタラクト範囲外になりました"));
	}
}

void AItemActor::Interact(ACharacter* Character)
{
	if (!bCanInteract || !Character || Character != PlayerInRange)
	{
		return;
	}

	UInventoryComponent* Inventory = Character->FindComponentByClass<UInventoryComponent>();
	if (Inventory)
	{
		bool bSuccess = Inventory->AddItem(ItemData, 1);
		if (bSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("アイテム「%s」を取得しました！"), *ItemData.Name.ToString());
			Destroy(); // 拾われたので消す
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("インベントリがいっぱいです！"));
		}
	}
}
