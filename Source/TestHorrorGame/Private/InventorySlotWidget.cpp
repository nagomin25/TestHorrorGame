#include "InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
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

    // アイテム表示の設定（2Dアイコンを優先）
    if (SlotData.Item.Icon && ItemIcon)
    {
        // 2Dアイコンを表示
        UE_LOG(LogTemp, Warning, TEXT("🖼️ Using 2D icon display (recommended)"));
        ItemIcon->SetBrushFromTexture(SlotData.Item.Icon);
        ItemIcon->SetVisibility(ESlateVisibility::Visible);
        
        
        UE_LOG(LogTemp, Warning, TEXT("✅ 2D icon displayed: %s"), *SlotData.Item.Icon->GetName());
    }
    else if (SlotData.Item.Mesh && ItemIcon)
    {
        // フォールバック：3Dメッシュを表示
        UE_LOG(LogTemp, Warning, TEXT("🎯 Fallback to 3D mesh display"));
        
        // C++側で直接メッシュを設定
        SetMeshDirect(SlotData.Item.Mesh);
        
        // C++実装のSetMeshForWidgetを呼び出し（Blueprint Event Graph代替）
        SetMeshForWidgetCpp(SlotData.Item.Mesh);
        
        // Blueprint側でもイベントを呼び出し（Blueprint側でのカスタマイズ用）
        SetMeshForWidget(SlotData.Item.Mesh);
        
        ItemIcon->SetVisibility(ESlateVisibility::Visible);
        
        // 2Dアイコンは非表示
        if (ItemIcon)
        {
            ItemIcon->SetVisibility(ESlateVisibility::Hidden);
        }
    }
    else if (ItemIcon)
    {
        // メッシュもアイコンもない場合は3Dウィジェットを空で表示
        UE_LOG(LogTemp, Warning, TEXT("⚠️ No mesh or icon available, showing empty 3D widget"));
        ItemIcon->SetVisibility(ESlateVisibility::Visible);
        
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

void UInventorySlotWidget::SetMeshDirect(UStaticMesh* NewMesh)
{
    UE_LOG(LogTemp, Warning, TEXT("🎯 SetMeshDirect called with mesh: %s"), 
        NewMesh ? *NewMesh->GetName() : TEXT("NULL"));

    if (!NewMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ SetMeshDirect called with null mesh"));
        return;
    }

    if (!ItemIcon)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ItemIcon is null in SetMeshDirect"));
        return;
    }
    
}

void UInventorySlotWidget::SetMeshForWidgetCpp(UStaticMesh* NewMesh)
{
    UE_LOG(LogTemp, Warning, TEXT("🎯 SetMeshForWidgetCpp called (C++ implementation of Blueprint Event Graph)"));
    
    if (!NewMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ SetMeshForWidgetCpp called with null mesh"));
        return;
    }

    if (!ItemIcon)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ItemIcon is null in SetMeshForWidgetCpp"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("📦 C++ Implementation - Setting mesh: %s"), *NewMesh->GetName());
}

