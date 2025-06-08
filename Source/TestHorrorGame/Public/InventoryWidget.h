#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UInventorySlotWidget;

UCLASS()
class TESTHORRORGAME_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // インベントリアイテムを設定
    UFUNCTION(BlueprintCallable)
    void SetInventory(const TArray<FInventorySlot>& Items);

protected:
    // インベントリグリッド（UMGでBindWidget）
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* InventoryGrid;

    // スロットウィジェットのクラス
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
    TSubclassOf<UInventorySlotWidget> SlotWidgetClass;
};
