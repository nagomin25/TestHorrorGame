#include "MainMenuGameMode.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

AMainMenuGameMode::AMainMenuGameMode()
{
    // メインメニューではプレイヤーキャラクターを無効化
    DefaultPawnClass = nullptr;
    
    // MainMenuWidgetの初期化
    MainMenuWidget = nullptr;
}

void AMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Warning, TEXT("🎮 MainMenuGameMode BeginPlay started"));
    
    // 現在のレベル名を取得
    FString CurrentLevelName = GetWorld()->GetMapName();
    CurrentLevelName = CurrentLevelName.Replace(TEXT("UEDPIE_0_"), TEXT("")); // エディタ用プレフィックスを削除
    
    UE_LOG(LogTemp, Warning, TEXT("🔍 Current level: %s"), *CurrentLevelName);
    
    // MainMenuレベルでのみメインメニューウィジェットを表示
    if (CurrentLevelName.Contains(TEXT("MainMenu")) || CurrentLevelName.Contains(TEXT("mainmenu")))
    {
        UE_LOG(LogTemp, Warning, TEXT("✅ MainMenu level detected - showing menu widget"));
        
        // メインメニューウィジェットを表示
        if (MainMenuWidgetClass && !MainMenuWidget)
        {
            MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
            if (MainMenuWidget)
            {
                UE_LOG(LogTemp, Warning, TEXT("🎮 MainMenuWidget created successfully"));
                MainMenuWidget->ShowMainMenu();
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("❌ Failed to create MainMenuWidget"));
            }
        }
        else if (!MainMenuWidgetClass)
        {
            UE_LOG(LogTemp, Error, TEXT("❌ MainMenuWidgetClass is not assigned in Blueprint"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("⏭️ Not MainMenu level - skipping menu widget"));
    }
    
    UE_LOG(LogTemp, Warning, TEXT("🎮 MainMenuGameMode BeginPlay completed"));
}