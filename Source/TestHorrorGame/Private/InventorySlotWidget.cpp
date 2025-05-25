#include "InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryMeshWidget.h"
#include "Blueprint/UserWidget.h"

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 3Dメッシュウィジェットの確認
	if (ItemMeshWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("✅ ItemMeshWidget found in blueprint"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ ItemMeshWidget is required but not found in blueprint!"));
	}
	
	// 2Dアイコンはオプショナル
	if (ItemIcon)
	{
		UE_LOG(LogTemp, Warning, TEXT("✅ ItemIcon available as fallback"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("💡 ItemIcon not found - using 3D-only mode"));
	}
}

void UInventorySlotWidget::Setup(const FInventorySlot& SlotData)
{
    CurrentSlotData = SlotData;

    // デバッグ：受け取ったデータを確認
    UE_LOG(LogTemp, Warning, TEXT("🔍 InventorySlotWidget::Setup called"));
    UE_LOG(LogTemp, Warning, TEXT("📋 ItemID: %s"), *SlotData.Item.ItemID.ToString());
    UE_LOG(LogTemp, Warning, TEXT("📋 Name: %s"), *SlotData.Item.Name.ToString());
    UE_LOG(LogTemp, Warning, TEXT("📋 Description: %s"), *SlotData.Item.Description.ToString());
    UE_LOG(LogTemp, Warning, TEXT("📋 Count: %d"), SlotData.Count);
    UE_LOG(LogTemp, Warning, TEXT("📋 Has Mesh: %s"), SlotData.Item.Mesh ? TEXT("Yes") : TEXT("No"));

    // アイテム個数の設定
    if (ItemCountText)
    {
        ItemCountText->SetText(FText::AsNumber(SlotData.Count));
        ItemCountText->SetVisibility(ESlateVisibility::Visible); // 常に表示
        UE_LOG(LogTemp, Warning, TEXT("✅ ItemCountText set to: %d"), SlotData.Count);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ItemCountText is nullptr!"));
    }

    // アイテム表示の設定（3Dメッシュを優先）
    if (SlotData.Item.Mesh && ItemMeshWidget)
    {
        // 3Dメッシュを表示
        UE_LOG(LogTemp, Warning, TEXT("🎯 Using 3D mesh display"));
        ItemMeshWidget->SetMesh(SlotData.Item.Mesh);
        ItemMeshWidget->SetVisibility(ESlateVisibility::Visible);
        
        // 2Dアイコンは非表示（存在する場合）
        if (ItemIcon)
        {
            ItemIcon->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    else if (SlotData.Item.Icon && ItemIcon)
    {
        // フォールバック：2Dアイコンを表示
        UE_LOG(LogTemp, Warning, TEXT("🖼️ Fallback to 2D icon display"));
        ItemIcon->SetBrushFromTexture(SlotData.Item.Icon);
        ItemIcon->SetVisibility(ESlateVisibility::Visible);
        
        // 3Dウィジェットは非表示
        if (ItemMeshWidget)
        {
            ItemMeshWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    else if (ItemMeshWidget)
    {
        // メッシュもアイコンもない場合は3Dウィジェットを空で表示
        UE_LOG(LogTemp, Warning, TEXT("⚠️ No mesh or icon available, showing empty 3D widget"));
        ItemMeshWidget->SetVisibility(ESlateVisibility::Visible);
        
        if (ItemIcon)
        {
            ItemIcon->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ No display widgets available!"));
    }

    // アイテム名の設定
    if (ItemNameText)
    {
        if (!SlotData.Item.Name.IsEmpty())
        {
            ItemNameText->SetText(SlotData.Item.Name);
            ItemNameText->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("✅ ItemNameText set to: %s"), *SlotData.Item.Name.ToString());
        }
        else
        {
            ItemNameText->SetText(FText::FromString(TEXT("Unknown Item")));
            ItemNameText->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("⚠️ ItemNameText set to default"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ItemNameText is nullptr!"));
    }

    // アイテム説明の設定
    if (ItemDescriptionText)
    {
        if (!SlotData.Item.Description.IsEmpty())
        {
            ItemDescriptionText->SetText(SlotData.Item.Description);
            ItemDescriptionText->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("✅ ItemDescriptionText set to: %s"), *SlotData.Item.Description.ToString());
        }
        else
        {
            ItemDescriptionText->SetText(FText::FromString(TEXT("説明なし")));
            ItemDescriptionText->SetVisibility(ESlateVisibility::Visible);
            UE_LOG(LogTemp, Warning, TEXT("⚠️ ItemDescriptionText set to default"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ItemDescriptionText is nullptr!"));
    }
}
