#include "InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// 初期化処理など（今は不要）
}

void UInventoryWidget::SetInventory(const TArray<FInventorySlot>& Items)
{
	UE_LOG(LogTemp, Warning, TEXT("📥 SetInventory called with %d items"), Items.Num());

	if (!InventoryGrid)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ InventoryGrid is null! Attempting to recreate..."));
		// UMGデザイナーで作成したウィジェットの場合は、バインドされているはずなので
		// エラーが発生した場合は親ウィジェットを再構築してみる
		NativeConstruct();
		
		// 再度チェック
		if (!InventoryGrid)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Still could not find InventoryGrid after reconstruction!"));
			return;
		}
	}

	if (!SlotWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ SlotWidgetClass is null! Did you assign it in BP or code?"));
		return;
	}

	InventoryGrid->ClearChildren();
	UE_LOG(LogTemp, Warning, TEXT("🧹 Cleared existing InventoryGrid children."));

	for (int32 i = 0; i < Items.Num(); ++i)
	{
		const int32 Row = i / 5;
		const int32 Column = i % 5;

		// 変数名を変更
		UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		if (!SlotWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Failed to create slot widget at index %d"), i);
			continue;
		}

		// データ渡し
		SlotWidget->Setup(Items[i]);
		UE_LOG(LogTemp, Warning, TEXT("✅ Setup slot %d (Count: %d)"), i, Items[i].Count);

		// グリッドに追加
		InventoryGrid->AddChildToUniformGrid(SlotWidget, Row, Column);
		UE_LOG(LogTemp, Warning, TEXT("🧩 Added slot to InventoryGrid at Row %d, Column %d"), Row, Column);
	}
}
