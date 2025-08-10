#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuWidget.h"
#include "MainMenuGameMode.generated.h"

UCLASS()
class TESTHORRORGAME_API AMainMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMainMenuGameMode();

protected:
    virtual void BeginPlay() override;

    // メインメニューウィジェットのクラス（Blueprintでアサイン）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
    TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

    // メインメニューウィジェットの実体
    UPROPERTY()
    UMainMenuWidget* MainMenuWidget;
};