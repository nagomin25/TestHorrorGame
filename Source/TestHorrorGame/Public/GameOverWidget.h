#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class TESTHORRORGAME_API UGameOverWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // ゲームオーバー画面を表示
    UFUNCTION(BlueprintCallable)
    void ShowGameOver();

protected:
    // ゲームオーバーテキスト
    UPROPERTY(meta = (BindWidget))
    UTextBlock* GameOverText;

    // リスタートボタン
    UPROPERTY(meta = (BindWidget))
    UButton* RestartButton;

    // メインメニューボタン
    UPROPERTY(meta = (BindWidget))
    UButton* MainMenuButton;

    // 終了ボタン
    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

private:
    UFUNCTION()
    void OnRestartButtonClicked();

    UFUNCTION()
    void OnMainMenuButtonClicked();

    UFUNCTION()
    void OnQuitButtonClicked();
};