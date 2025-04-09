#include "MainMenuHUD.h"
#include "Blueprint/UserWidget.h"

void AMainMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuWidgetClass)
    {
        UUserWidget* MainMenu = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
        if (MainMenu)
        {
            MainMenu->AddToViewport();
        }
    }
}
