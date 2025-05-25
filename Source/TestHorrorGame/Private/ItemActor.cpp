#include "ItemActor.h"
#include "TestHorrorGame/TestHorrorGameCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
#include "UObject/ConstructorHelpers.h"

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

	// デフォルトのアイテムデータを設定（テスト用）
	ItemData.ItemID = FName("DefaultItem");
	ItemData.Name = FText::FromString("テストアイテム");
	ItemData.Description = FText::FromString("これはテスト用のアイテムです。説明がここに表示されます。");
	ItemData.MaxStack = 99;
	ItemData.Icon = nullptr; // アイコンはブループリントで設定
	
	// テスト用：デフォルトキューブメッシュを使用
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Engine/BasicShapes/Cube"));
	if (DefaultMesh.Succeeded())
	{
		ItemData.Mesh = DefaultMesh.Object;
		UE_LOG(LogTemp, Warning, TEXT("✅ Default cube mesh assigned for testing"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("⚠️ Could not find default cube mesh"));
	}
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

	// デバッグ：送信するアイテムデータを確認
	UE_LOG(LogTemp, Warning, TEXT("🎮 ItemActor::Interact - Sending item data:"));
	UE_LOG(LogTemp, Warning, TEXT("📋 ItemID: %s"), *ItemData.ItemID.ToString());
	UE_LOG(LogTemp, Warning, TEXT("📋 Name: %s"), *ItemData.Name.ToString());
	UE_LOG(LogTemp, Warning, TEXT("📋 Description: %s"), *ItemData.Description.ToString());
	UE_LOG(LogTemp, Warning, TEXT("📋 MaxStack: %d"), ItemData.MaxStack);

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
