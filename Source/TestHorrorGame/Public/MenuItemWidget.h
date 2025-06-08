#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "MenuItemWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UMenuWidget;

UCLASS()
class TESTHORRORGAME_API UMenuItemWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // アイテムデータを設定
    UFUNCTION(BlueprintCallable)
    void SetupMenuItem(const FInventorySlot& SlotData, UMenuWidget* ParentMenu);

protected:
    // クリック可能なボタン
    UPROPERTY(meta = (BindWidget))
    UButton* ItemButton;

    // アイテムアイコン
    UPROPERTY(meta = (BindWidget))
    UImage* ItemIcon;

    // アイテム名
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemName;

    // アイテム数
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ItemCount;

    // 保持するアイテムデータ
    UPROPERTY()
    FInventorySlot ItemSlot;

    // 親メニューへの参照
    UPROPERTY()
    UMenuWidget* ParentMenuWidget;

private:
    UFUNCTION()
    void OnItemButtonClicked();
};
