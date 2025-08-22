#include "MainMenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "Blueprint/WidgetTree.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    UE_LOG(LogTemp, Warning, TEXT("🔍 MainMenuWidget NativeConstruct started"));
    
    // BindWidgetが失敗した場合の代替手段として手動でウィジェットを検索
    if (!StartGameButton)
    {
        // 複数の可能な名前で検索
        TArray<FString> PossibleNames = {TEXT("StartGameButton"), TEXT("StartButton"), TEXT("Start")};
        for (const FString& Name : PossibleNames)
        {
            StartGameButton = Cast<UButton>(GetWidgetFromName(*Name));
            if (StartGameButton)
            {
                UE_LOG(LogTemp, Warning, TEXT("🔍 Found StartGameButton with name: %s"), *Name);
                break;
            }
        }
        
        // それでも見つからない場合は、すべてのButtonを検索
        if (!StartGameButton)
        {
            TArray<UWidget*> AllChildren;
            WidgetTree->GetAllWidgets(AllChildren);
            for (UWidget* Child : AllChildren)
            {
                if (UButton* Button = Cast<UButton>(Child))
                {
                    FString ButtonName = Button->GetName();
                    if (ButtonName.Contains(TEXT("Start")) || ButtonName.Contains(TEXT("Game")))
                    {
                        StartGameButton = Button;
                        UE_LOG(LogTemp, Warning, TEXT("🔍 Found StartGameButton by search: %s"), *ButtonName);
                        break;
                    }
                }
            }
        }
    }
    
    if (!SettingsButton)
    {
        TArray<FString> PossibleNames = {TEXT("SettingsButton"), TEXT("Settings"), TEXT("Option")};
        for (const FString& Name : PossibleNames)
        {
            SettingsButton = Cast<UButton>(GetWidgetFromName(*Name));
            if (SettingsButton) break;
        }
        
        if (!SettingsButton)
        {
            TArray<UWidget*> AllChildren;
            WidgetTree->GetAllWidgets(AllChildren);
            for (UWidget* Child : AllChildren)
            {
                if (UButton* Button = Cast<UButton>(Child))
                {
                    FString ButtonName = Button->GetName();
                    if (ButtonName.Contains(TEXT("Setting")) || ButtonName.Contains(TEXT("Option")))
                    {
                        SettingsButton = Button;
                        break;
                    }
                }
            }
        }
    }
    
    if (!QuitButton)
    {
        TArray<FString> PossibleNames = {TEXT("QuitButton"), TEXT("Quit"), TEXT("Exit")};
        for (const FString& Name : PossibleNames)
        {
            QuitButton = Cast<UButton>(GetWidgetFromName(*Name));
            if (QuitButton) break;
        }
        
        if (!QuitButton)
        {
            TArray<UWidget*> AllChildren;
            WidgetTree->GetAllWidgets(AllChildren);
            for (UWidget* Child : AllChildren)
            {
                if (UButton* Button = Cast<UButton>(Child))
                {
                    FString ButtonName = Button->GetName();
                    if (ButtonName.Contains(TEXT("Quit")) || ButtonName.Contains(TEXT("Exit")))
                    {
                        QuitButton = Button;
                        break;
                    }
                }
            }
        }
    }
    
    if (!TitleText)
    {
        TArray<FString> PossibleNames = {TEXT("TitleText"), TEXT("Title"), TEXT("GameTitle")};
        for (const FString& Name : PossibleNames)
        {
            TitleText = Cast<UTextBlock>(GetWidgetFromName(*Name));
            if (TitleText) break;
        }
        
        if (!TitleText)
        {
            TArray<UWidget*> AllChildren;
            WidgetTree->GetAllWidgets(AllChildren);
            for (UWidget* Child : AllChildren)
            {
                if (UTextBlock* TextBlock = Cast<UTextBlock>(Child))
                {
                    FString TextName = TextBlock->GetName();
                    if (TextName.Contains(TEXT("Title")))
                    {
                        TitleText = TextBlock;
                        break;
                    }
                }
            }
        }
    }
    
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
    
    // デバッグ用：すべてのウィジェット名を出力
    DebugPrintAllWidgetNames();
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
    // ログ削除
}

void UMainMenuWidget::OnStartGameButtonReleased()
{
    // ログ削除
}

void UMainMenuWidget::OnStartGameButtonClicked()
{
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
        }
        
        // メインゲームレベル(testWorld)をロード
        UGameplayStatics::OpenLevel(this, FName(*FString("testWorld")));
    }
}

void UMainMenuWidget::OnSettingsButtonClicked()
{
    // 設定画面の実装（将来的に実装）
}

void UMainMenuWidget::OnQuitButtonClicked()
{
    // ゲームを終了
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PlayerController = World->GetFirstPlayerController())
        {
            UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, false);
        }
    }
}

void UMainMenuWidget::DebugPrintAllWidgetNames()
{
    // デバッグ機能は無効化
}