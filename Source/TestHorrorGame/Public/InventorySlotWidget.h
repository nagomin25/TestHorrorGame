#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "Engine/StaticMesh.h"
#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;
class UUserWidget;

UCLASS(Blueprintable)
class TESTHORRORGAME_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Setup(const FInventorySlot& SlotData);

	// Blueprint側でメッシュ設定を処理するためのイベント
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void SetMeshForWidget(UStaticMesh* NewMesh);

	// C++側でも直接メッシュを設定できる関数
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SetMeshDirect(UStaticMesh* NewMesh);

	// SetMeshForWidgetのC++実装版（BlueprintとC++両方で使用可能）
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SetMeshForWidgetCpp(UStaticMesh* NewMesh);

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

private:
	// 現在のアイテムデータ
	FInventorySlot CurrentSlotData;
};
