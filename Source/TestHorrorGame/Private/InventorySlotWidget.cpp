#include "InventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventorySlotWidget::Setup(const FInventorySlot& SlotData)
{
    if (ItemCountText)
    {
        ItemCountText->SetText(FText::AsNumber(SlotData.Count));
        ItemCountText->SetVisibility(SlotData.Count > 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }

    if (SlotData.Item.Icon && ItemIcon)
    {
        ItemIcon->SetBrushFromTexture(SlotData.Item.Icon);
    }
    else if (ItemIcon)
    {
        ItemIcon->SetBrush(FSlateBrush());
    }
}
