#include "GameOverWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

void UGameOverWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // ボタンのイベントをバインド
    if (RestartButton)
    {
        RestartButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnRestartButtonClicked);
    }
    
    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnMainMenuButtonClicked);
    }
    
    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnQuitButtonClicked);
    }
    
    // ゲームオーバーテキストを設定
    if (GameOverText)
    {
        GameOverText->SetText(FText::FromString(TEXT("ゲームオーバー")));
    }
    
    UE_LOG(LogTemp, Warning, TEXT("💀 GameOverWidget constructed successfully"));
}

void UGameOverWidget::NativeDestruct()
{
    // デリゲートを安全にアンバインド
    if (RestartButton)
    {
        RestartButton->OnClicked.RemoveAll(this);
    }
    
    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.RemoveAll(this);
    }
    
    if (QuitButton)
    {
        QuitButton->OnClicked.RemoveAll(this);
    }
    
    Super::NativeDestruct();
}

void UGameOverWidget::ShowGameOver()
{
    UE_LOG(LogTemp, Warning, TEXT("💀 Showing Game Over screen"));
    
    // ウィジェットをビューポートに追加
    AddToViewport();
    
    // マウスカーソルを表示してUIモードに設定
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            PlayerController->bShowMouseCursor = true;
            PlayerController->SetInputMode(FInputModeUIOnly());
            
            // ゲームを一時停止
            PlayerController->SetPause(true);
            
            UE_LOG(LogTemp, Warning, TEXT("🎮 Game Over UI mode activated"));
        }
    }
}

void UGameOverWidget::OnRestartButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("🔄 Restart button clicked"));
    
    // ゲームオーバー画面を削除
    RemoveFromParent();
    
    // 入力モードを正常に戻してからレベルをリロード
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            // ゲームを再開
            PlayerController->SetPause(false);
            
            // 入力モードをゲームモードに戻す
            PlayerController->bShowMouseCursor = false;
            PlayerController->SetInputMode(FInputModeGameOnly());
            
            UE_LOG(LogTemp, Warning, TEXT("🎮 Input mode restored to Game Only"));
        }
        
        // 現在のレベル名を取得してリロード
        FString CurrentLevelName = World->GetMapName();
        CurrentLevelName = CurrentLevelName.Replace(TEXT("UEDPIE_0_"), TEXT("")); // エディタ用プレフィックスを削除
        
        UE_LOG(LogTemp, Warning, TEXT("🔄 Restarting level: %s"), *CurrentLevelName);
        UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
    }
}

void UGameOverWidget::OnMainMenuButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("🏠 Main Menu button clicked"));
    
    // ゲームオーバー画面を削除
    RemoveFromParent();
    
    // ゲームを再開してからメインメニューレベルに移動
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            PlayerController->SetPause(false);
            
            // 入力モードをUIモードに設定（メインメニュー用）
            PlayerController->bShowMouseCursor = true;
            PlayerController->SetInputMode(FInputModeUIOnly());
            
            UE_LOG(LogTemp, Warning, TEXT("🎮 Input mode set to UI Only for Main Menu"));
        }
        
        // メインメニューレベルを開く
        UE_LOG(LogTemp, Warning, TEXT("🏠 Loading MainMenu level..."));
        UGameplayStatics::OpenLevel(this, TEXT("MainMenu"));
    }
}

void UGameOverWidget::OnQuitButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("🚪 Quit button clicked"));
    
    // ゲームを終了
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            PlayerController->SetPause(false);
            
            // ゲームを終了
            UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
        }
    }
}