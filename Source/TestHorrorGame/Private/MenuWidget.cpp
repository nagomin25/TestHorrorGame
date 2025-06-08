#include "MenuWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MenuItemWidget.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "TestHorrorGame/TestHorrorGameCharacter.h"
#include "ItemData.h"

void UMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 閉じるボタンのイベントをバインド
    if (CloseButton)
    {
        CloseButton->OnClicked.AddDynamic(this, &UMenuWidget::OnCloseButtonClicked);
    }
    
    // 初期状態では詳細表示エリアを空にする
    if (DetailItemIcon)
    {
        DetailItemIcon->SetBrushFromTexture(nullptr);
    }
    if (DetailItemName)
    {
        DetailItemName->SetText(FText::FromString(TEXT("アイテムを選択してください")));
    }
    if (DetailItemDescription)
    {
        DetailItemDescription->SetText(FText::FromString(TEXT("")));
    }
    if (DetailItemCount)
    {
        DetailItemCount->SetText(FText::FromString(TEXT("")));
    }
}

void UMenuWidget::NativeDestruct()
{
    // デリゲートを安全にアンバインド
    if (CloseButton)
    {
        CloseButton->OnClicked.RemoveAll(this);
    }
    
    Super::NativeDestruct();
}

void UMenuWidget::SetInventory(const TArray<FInventorySlot>& Items)
{
    UE_LOG(LogTemp, Warning, TEXT("📋 MenuWidget: SetInventory called with %d items"), Items.Num());

    if (!ItemListScrollBox)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ ItemListScrollBox is null!"));
        return;
    }

    if (!MenuItemWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ MenuItemWidgetClass is null! Did you assign it in BP?"));
        return;
    }

    // 既存のアイテムを安全にクリア
    if (ItemListScrollBox->GetChildrenCount() > 0)
    {
        // 各子ウィジェットを個別に削除してデリゲートを適切にクリア
        TArray<UWidget*> ChildWidgets = ItemListScrollBox->GetAllChildren();
        for (UWidget* Child : ChildWidgets)
        {
            if (UMenuItemWidget* MenuItemWidget = Cast<UMenuItemWidget>(Child))
            {
                // 明示的にRemoveFromParentを呼び出してNativeDestructを確実に実行
                MenuItemWidget->RemoveFromParent();
            }
        }
    }
    ItemListScrollBox->ClearChildren();
    UE_LOG(LogTemp, Warning, TEXT("🧹 Cleared existing menu items safely."));

    // 各アイテムに対してメニューアイテムウィジェットを作成
    for (int32 i = 0; i < Items.Num(); ++i)
    {
        const FInventorySlot& CurrentSlot = Items[i];
        
        // 空のスロットはスキップ
        if (CurrentSlot.Count <= 0)
        {
            continue;
        }

        UMenuItemWidget* MenuItemWidget = CreateWidget<UMenuItemWidget>(this, MenuItemWidgetClass);
        if (!MenuItemWidget)
        {
            UE_LOG(LogTemp, Error, TEXT("❌ Failed to create menu item widget at index %d"), i);
            continue;
        }

        // メニューアイテムを設定
        MenuItemWidget->SetupMenuItem(CurrentSlot, this);
        UE_LOG(LogTemp, Warning, TEXT("✅ Setup menu item %d: %s (Count: %d)"), 
               i, *CurrentSlot.Item.Name.ToString(), CurrentSlot.Count);

        // スクロールボックスに追加
        ItemListScrollBox->AddChild(MenuItemWidget);
    }

    UE_LOG(LogTemp, Warning, TEXT("📋 MenuWidget: Finished setting up inventory"));
}

void UMenuWidget::OnItemSelected(const FInventorySlot& SelectedSlot)
{
    UE_LOG(LogTemp, Warning, TEXT("🎯 Item selected: %s"), *SelectedSlot.Item.Name.ToString());
    
    CurrentSelectedItem = SelectedSlot;

    // 詳細表示エリアを更新
    if (DetailItemIcon && SelectedSlot.Item.Icon)
    {
        DetailItemIcon->SetBrushFromTexture(SelectedSlot.Item.Icon);
    }

    if (DetailItemName)
    {
        DetailItemName->SetText(SelectedSlot.Item.Name);
    }

    if (DetailItemDescription)
    {
        DetailItemDescription->SetText(SelectedSlot.Item.Description);
    }

    if (DetailItemCount)
    {
        FString CountText = FString::Printf(TEXT("所持数: %d"), SelectedSlot.Count);
        if (SelectedSlot.Item.MaxStack > 1)
        {
            CountText += FString::Printf(TEXT(" / %d"), SelectedSlot.Item.MaxStack);
        }
        DetailItemCount->SetText(FText::FromString(CountText));
    }
}

void UMenuWidget::CloseMenu()
{
    UE_LOG(LogTemp, Warning, TEXT("🚪 Closing menu"));
    
    // まずウィジェットを削除
    RemoveFromParent();
    
    // PlayerControllerとInputSubsystemを取得してInput Contextを手動で切り替え
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            // Input Mapping Contextを通常に戻す
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
            {
                // キャラクターからMapping Contextを取得
                if (APawn* PlayerPawn = PlayerController->GetPawn())
                {
                    if (ATestHorrorGameCharacter* Character = Cast<ATestHorrorGameCharacter>(PlayerPawn))
                    {
                        // MenuMappingContextを削除してDefaultMappingContextを追加
                        if (Character->MenuMappingContext)
                        {
                            Subsystem->RemoveMappingContext(Character->MenuMappingContext);
                        }
                        if (Character->DefaultMappingContext)
                        {
                            Subsystem->AddMappingContext(Character->DefaultMappingContext, 0);
                        }
                        UE_LOG(LogTemp, Warning, TEXT("🎮 Switched back to Default Input Context"));
                    }
                }
            }
            
            // マウスカーソルを非表示
            PlayerController->bShowMouseCursor = false;
            PlayerController->SetInputMode(FInputModeGameOnly());
        }
    }
}

void UMenuWidget::OnCloseButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("🖱️ Close button clicked"));
    CloseMenu();
}
