#include "MenuItemWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "MenuWidget.h"
#include "Engine/Texture2D.h"

void UMenuItemWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // ボタンのクリックイベントをバインド
    if (ItemButton)
    {
        ItemButton->OnClicked.AddDynamic(this, &UMenuItemWidget::OnItemButtonClicked);
    }
}

void UMenuItemWidget::NativeDestruct()
{
    // デリゲートを安全にアンバインド
    if (ItemButton)
    {
        ItemButton->OnClicked.RemoveAll(this);
    }
    
    // 親への参照をクリア
    ParentMenuWidget = nullptr;
    
    Super::NativeDestruct();
}

void UMenuItemWidget::SetupMenuItem(const FInventorySlot& SlotData, UMenuWidget* ParentMenu)
{
    ItemSlot = SlotData;
    ParentMenuWidget = ParentMenu;

    UE_LOG(LogTemp, Warning, TEXT("🔧 Setting up menu item: %s"), *SlotData.Item.Name.ToString());

    // アイコンを設定
    if (ItemIcon && SlotData.Item.Icon)
    {
        ItemIcon->SetBrushFromTexture(SlotData.Item.Icon);
        UE_LOG(LogTemp, Warning, TEXT("✅ Set icon for item: %s"), *SlotData.Item.Name.ToString());
    }
    else if (ItemIcon)
    {
        // デフォルトアイコンまたは空の状態
        ItemIcon->SetBrushFromTexture(nullptr);
        UE_LOG(LogTemp, Warning, TEXT("⚠️ No icon available for item: %s"), *SlotData.Item.Name.ToString());
    }

    // アイテム名を設定
    if (ItemName)
    {
        ItemName->SetText(SlotData.Item.Name);
    }

    // アイテム数を設定
    if (ItemCount)
    {
        FString CountText = FString::Printf(TEXT("x%d"), SlotData.Count);
        ItemCount->SetText(FText::FromString(CountText));
    }

    UE_LOG(LogTemp, Warning, TEXT("✅ Menu item setup complete: %s (Count: %d)"), 
           *SlotData.Item.Name.ToString(), SlotData.Count);
}

void UMenuItemWidget::OnItemButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("🖱️ Menu item clicked: %s"), *ItemSlot.Item.Name.ToString());
    
    if (ParentMenuWidget)
    {
        ParentMenuWidget->OnItemSelected(ItemSlot);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ParentMenuWidget is null!"));
    }
}
