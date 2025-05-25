#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;
class UInventoryMeshWidget;

UCLASS(Blueprintable)
class TESTHORRORGAME_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Setup(const FInventorySlot& SlotData);

protected:
	virtual void NativeConstruct() override;

	// 2Dアイコン（オプショナル - 3Dメッシュがない場合のフォールバック用）
	UPROPERTY(meta = (BindWidgetOptional))
	UImage* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescriptionText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;

	// 3Dメッシュ表示用ウィジェット（メイン表示）
	UPROPERTY(meta = (BindWidget))
	UInventoryMeshWidget* ItemMeshWidget;

	// 3Dメッシュ表示ウィジェットのクラス（Blueprintで設定）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UInventoryMeshWidget> MeshWidgetClass;

private:
	// 現在のアイテムデータ
	FInventorySlot CurrentSlotData;
};
