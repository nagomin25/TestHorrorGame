#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class TESTHORRORGAME_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // メインメニューを表示
    UFUNCTION(BlueprintCallable)
    void ShowMainMenu();
    
    // デバッグ用：全ての子ウィジェット名を出力
    UFUNCTION(BlueprintCallable)
    void DebugPrintAllWidgetNames();

protected:
    // タイトルテキスト
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* TitleText;

    // ゲーム開始ボタン
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UButton* StartGameButton;

    // 設定ボタン
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UButton* SettingsButton;

    // 終了ボタン
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UButton* QuitButton;

private:
    UFUNCTION()
    void OnStartGameButtonClicked();

    UFUNCTION()
    void OnStartGameButtonPressed();

    UFUNCTION()
    void OnStartGameButtonReleased();

    UFUNCTION()
    void OnSettingsButtonClicked();

    UFUNCTION()
    void OnQuitButtonClicked();
};