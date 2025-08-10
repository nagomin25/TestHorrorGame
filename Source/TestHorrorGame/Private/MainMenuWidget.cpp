#include "MainMenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    UE_LOG(LogTemp, Warning, TEXT("🔍 MainMenuWidget NativeConstruct started"));
    
    // 各ウィジェットの存在チェック
    UE_LOG(LogTemp, Warning, TEXT("🔍 StartGameButton is %s"), StartGameButton ? TEXT("Valid") : TEXT("NULL"));
    UE_LOG(LogTemp, Warning, TEXT("🔍 SettingsButton is %s"), SettingsButton ? TEXT("Valid") : TEXT("NULL"));
    UE_LOG(LogTemp, Warning, TEXT("🔍 QuitButton is %s"), QuitButton ? TEXT("Valid") : TEXT("NULL"));
    UE_LOG(LogTemp, Warning, TEXT("🔍 TitleText is %s"), TitleText ? TEXT("Valid") : TEXT("NULL"));
    
    // ボタンのイベントをバインド
    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGameButtonClicked);
        UE_LOG(LogTemp, Warning, TEXT("✅ StartGameButton event bound successfully"));
        
        // OnPressedとOnReleasedもバインドしてテスト
        StartGameButton->OnPressed.AddDynamic(this, &UMainMenuWidget::OnStartGameButtonPressed);
        StartGameButton->OnReleased.AddDynamic(this, &UMainMenuWidget::OnStartGameButtonReleased);
        
        // ボタンの状態をチェック
        UE_LOG(LogTemp, Warning, TEXT("🔍 StartGameButton IsEnabled: %s"), StartGameButton->GetIsEnabled() ? TEXT("true") : TEXT("false"));
        UE_LOG(LogTemp, Warning, TEXT("🔍 StartGameButton Visibility: %d"), (int32)StartGameButton->GetVisibility());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ StartGameButton is NULL - check Blueprint variable name"));
    }
    
    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsButtonClicked);
        UE_LOG(LogTemp, Warning, TEXT("✅ SettingsButton event bound successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ SettingsButton is NULL"));
    }
    
    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitButtonClicked);
        UE_LOG(LogTemp, Warning, TEXT("✅ QuitButton event bound successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ QuitButton is NULL"));
    }
    
    // タイトルテキストを設定
    if (TitleText)
    {
        TitleText->SetText(FText::FromString(TEXT("ホラーゲーム")));
        UE_LOG(LogTemp, Warning, TEXT("✅ TitleText set successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ TitleText is NULL"));
    }
    
    UE_LOG(LogTemp, Warning, TEXT("🎮 MainMenuWidget constructed successfully"));
}

void UMainMenuWidget::NativeDestruct()
{
    // デリゲートを安全にアンバインド
    if (StartGameButton)
    {
        StartGameButton->OnClicked.RemoveAll(this);
    }
    
    if (SettingsButton)
    {
        SettingsButton->OnClicked.RemoveAll(this);
    }
    
    if (QuitButton)
    {
        QuitButton->OnClicked.RemoveAll(this);
    }
    
    Super::NativeDestruct();
}

void UMainMenuWidget::ShowMainMenu()
{
    UE_LOG(LogTemp, Warning, TEXT("🎮 Showing Main Menu screen"));
    
    // ウィジェットをビューポートに追加
    AddToViewport();
    
    // ウィジェットにフォーカスを設定
    SetFocus();
    UE_LOG(LogTemp, Warning, TEXT("🔍 Widget focus set"));
    
    // マウスカーソルを表示してUIモードに設定
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            PlayerController->bShowMouseCursor = true;
            PlayerController->SetInputMode(FInputModeUIOnly());
            
            UE_LOG(LogTemp, Warning, TEXT("🎮 Main Menu UI mode activated"));
            UE_LOG(LogTemp, Warning, TEXT("🔍 Mouse cursor visible: %s"), PlayerController->bShowMouseCursor ? TEXT("true") : TEXT("false"));
        }
    }
    
    // 追加のウィジェット状態チェック
    UE_LOG(LogTemp, Warning, TEXT("🔍 Widget IsInViewport: %s"), IsInViewport() ? TEXT("true") : TEXT("false"));
    
    if (UWorld* CurrentWorld = GetWorld())
    {
        if (APlayerController* PC = CurrentWorld->GetFirstPlayerController())
        {
            UE_LOG(LogTemp, Warning, TEXT("🔍 Widget HasUserFocus: %s"), HasUserFocus(PC) ? TEXT("true") : TEXT("false"));
        }
    }
}

void UMainMenuWidget::OnStartGameButtonPressed()
{
    UE_LOG(LogTemp, Error, TEXT("🖱️ START BUTTON PRESSED"));
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Button Pressed"));
    }
}

void UMainMenuWidget::OnStartGameButtonReleased()
{
    UE_LOG(LogTemp, Error, TEXT("🖱️ START BUTTON RELEASED"));
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Button Released"));
    }
}

void UMainMenuWidget::OnStartGameButtonClicked()
{
    UE_LOG(LogTemp, Error, TEXT("🎯🎯🎯 START GAME BUTTON CLICKED - THIS SHOULD BE VISIBLE 🎯🎯🎯"));
    
    // デバッグ用の画面メッセージも表示
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("START BUTTON CLICKED!"));
    }
    
    // メインメニューを削除
    RemoveFromParent();
    
    // 入力モードをゲームモードに戻してからメインゲームレベルをロード
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            // 入力モードをゲームモードに戻す
            PlayerController->bShowMouseCursor = false;
            PlayerController->SetInputMode(FInputModeGameOnly());
            
            UE_LOG(LogTemp, Error, TEXT("🎮 Input mode restored to Game Only"));
        }
        
        // メインゲームレベル(TestWorld)をロード
        UE_LOG(LogTemp, Error, TEXT("🎯 Loading TestWorld level..."));
        UGameplayStatics::OpenLevel(this, TEXT("TestWorld"));
    }
}

void UMainMenuWidget::OnSettingsButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("⚙️ Settings button clicked"));
    
    // 設定画面の実装（将来的に実装）
    UE_LOG(LogTemp, Warning, TEXT("⚙️ Settings screen not implemented yet"));
}

void UMainMenuWidget::OnQuitButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("🚪 Quit button clicked"));
    
    // ゲームを終了
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            // ゲームを終了
            UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
        }
    }
}