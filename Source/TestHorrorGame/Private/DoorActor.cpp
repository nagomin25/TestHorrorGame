#include "DoorActor.h"
#include "TestHorrorGame/TestHorrorGameCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ADoorActor::ADoorActor()
{
	PrimaryActorTick.bCanEverTick = false;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	// デフォルトのドアメッシュを設定
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultDoorMesh(TEXT("/Game/PSXSurvivalHorrorFBX/PSXSurvivalHorrorFBX/MESH-Door"));
	if (DefaultDoorMesh.Succeeded())
	{
		
		DoorMesh->SetStaticMesh(DefaultDoorMesh.Object);
		UE_LOG(LogTemp, Warning, TEXT("✅ Default door mesh assigned"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("⚠️ Could not find default door mesh"));
	}

	// デフォルトのドアマテリアルを設定
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultDoorMaterial(TEXT("/Game/PSXSurvivalHorrorFBX/PSXSurvivalHorrorFBX/MAT-Door"));
	if (DefaultDoorMaterial.Succeeded())
	{
		DoorMesh->SetMaterial(0, DefaultDoorMaterial.Object);
		UE_LOG(LogTemp, Warning, TEXT("✅ Default door material assigned"));
	}

	// ドアメッシュのコリジョン設定
	DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DoorMesh->SetCollisionResponseToAllChannels(ECR_Block);
	DoorMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(200.f);
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	bCanInteract = false;
	PlayerInRange = nullptr;

	RequiredKeyID = FName("key");
}

void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ADoorActor::OnOverlapBegin);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ADoorActor::OnOverlapEnd);
}

void ADoorActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
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
		Character->InteractableDoors.AddUnique(this);
		
		UE_LOG(LogTemp, Warning, TEXT("🚪 ドアがインタラクト可能になりました"));
	}
}

void ADoorActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
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
		Character->InteractableDoors.Remove(this);
		
		UE_LOG(LogTemp, Warning, TEXT("🚪 ドアがインタラクト範囲外になりました"));
	}
}

void ADoorActor::Interact(ACharacter* Character)
{
	if (!bCanInteract || !Character || Character != PlayerInRange)
	{
		return;
	}

	UInventoryComponent* Inventory = Character->FindComponentByClass<UInventoryComponent>();
	if (!Inventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ インベントリコンポーネントが見つかりません"));
		return;
	}

	if (Inventory->HasItem(RequiredKeyID))
	{
		UE_LOG(LogTemp, Warning, TEXT("🎉 鍵を持っています！ゲームクリア！"));
		GameClear();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("🔒 鍵が必要です"));
	}
}

void ADoorActor::GameClear()
{
	UE_LOG(LogTemp, Warning, TEXT("🏆 ゲームクリア！"));
	
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->SetShowMouseCursor(true);
		
		FInputModeUIOnly InputMode;
		PC->SetInputMode(InputMode);
		
		UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenuMap"));
		
		UE_LOG(LogTemp, Warning, TEXT("✅ ゲームクリア処理完了 - タイトル画面に戻ります"));
	}
}