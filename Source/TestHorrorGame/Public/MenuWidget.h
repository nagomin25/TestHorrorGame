#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "MenuWidget.generated.h"

class UScrollBox;
class UMenuItemWidget;
class UImage;
class UTextBlock;
class UButton;

UCLASS()
class TESTHORRORGAME_API UMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // インベントリアイテムを設定
    UFUNCTION(BlueprintCallable)
    void SetInventory(const TArray<FInventorySlot>& Items);

    // アイテムが選択された時の処理
    UFUNCTION(BlueprintCallable)
    void OnItemSelected(const FInventorySlot& SelectedSlot);

    // メニューを閉じる
    UFUNCTION(BlueprintCallable)
    void CloseMenu();

protected:
    // 右側のアイテムリスト
    UPROPERTY(meta = (BindWidget))
    UScrollBox* ItemListScrollBox;

    // 左側の詳細表示エリア
    UPROPERTY(meta = (BindWidget))
    UImage* DetailItemIcon;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DetailItemName;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DetailItemDescription;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DetailItemCount;

    // 閉じるボタン
    UPROPERTY(meta = (BindWidget))
    UButton* CloseButton;

    // メニューアイテムウィジェットのクラス
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Menu")
    TSubclassOf<UMenuItemWidget> MenuItemWidgetClass;

    // 現在選択されているアイテム
    UPROPERTY()
    FInventorySlot CurrentSelectedItem;

private:
    UFUNCTION()
    void OnCloseButtonClicked();
};
