// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestHorrorGameCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.h"
#include "MenuWidget.h"
#include "ItemActor.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ATestHorrorGameCharacter

ATestHorrorGameCharacter::ATestHorrorGameCharacter()
{

	// コンポーネントを作成してアタッチ
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	
	// InputActionの初期設定（Blueprintでオーバーライド可能）
	static ConstructorHelpers::FObjectFinder<UInputAction> DefaultOpenInventoryAction(TEXT("/Game/ThirdPerson/Input/Actions/IA_OpenInventory"));
	if (DefaultOpenInventoryAction.Succeeded())
	{
		OpenInventoryAction = DefaultOpenInventoryAction.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> DefaultInteractAction(TEXT("/Game/ThirdPerson/Input/Actions/IA_Interact"));
	if (DefaultInteractAction.Succeeded())
	{
		InteractAction = DefaultInteractAction.Object;
	}
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATestHorrorGameCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (!DefaultMappingContext)
			{
				UE_LOG(LogTemp, Error, TEXT("❌ DefaultMappingContext is NULL! Input will not work."));
				return;
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Adding DefaultMappingContext to input system"));
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ EnhancedInputLocalPlayerSubsystem not found!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is not a PlayerController"));
	}
}

void ATestHorrorGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// インプットアクションの確認
		if (!MoveAction)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ MoveAction is not set!"));
		}
		
		if (!LookAction)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ LookAction is not set!"));
		}
		
		if (!OpenInventoryAction)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ OpenInventoryAction is not set! Inventory cannot be opened."));
		}
		
		if (!InteractAction)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ InteractAction is not set! Cannot interact with items."));
		}
		
		// Moving
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATestHorrorGameCharacter::Move);
		}

		// Looking
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATestHorrorGameCharacter::Look);
		}

		// インベントリアクションのバインド（新しいMenuWidgetを使用）
		if (OpenInventoryAction)
		{
			UE_LOG(LogTemp, Warning, TEXT("Binding OpenInventoryAction to ToggleMenu"));
			EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &ATestHorrorGameCharacter::ToggleMenu);
		}
		
		// インタラクトアクションのバインド
		if (InteractAction)
		{
			UE_LOG(LogTemp, Warning, TEXT("Binding InteractAction"));
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ATestHorrorGameCharacter::Interact);
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATestHorrorGameCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 前進・後退
	if (FMath::Abs(MovementVector.Y) > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	}

	// 左右旋回（キャラ自体をYAWのみで回転）
	if (FMath::Abs(MovementVector.X) > KINDA_SMALL_NUMBER)
	{
		const float TurnRate = 130.0f; // 必要に応じて調整
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		float YawInput = MovementVector.X * TurnRate * DeltaTime;

		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += YawInput;
		SetActorRotation(NewRotation);
	}
}



void ATestHorrorGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void ATestHorrorGameCharacter::ToggleInventory()
{
	UE_LOG(LogTemp, Warning, TEXT("ToggleInventory called!"));

	// WidgetClassが設定されているか確認
	if (!InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ InventoryWidgetClass is not set in Blueprint!"));
		return;
	}

	// InventoryComponentの中身チェック
	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ InventoryComponent is null!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Inventory contains %d items"), InventoryComponent->Inventory.Num());

	// WidgetがNull、かつWidgetClassが設定されている場合は新しく作成
	if (!InventoryWidget && InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating new InventoryWidget"));
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);

		if (!InventoryWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Failed to create InventoryWidget!"));
			return;
		}

		InventoryWidget->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("✅ InventoryWidget Added to Viewport (initial)"));
		InventoryWidget->SetInventory(InventoryComponent->Inventory);
		return;
	}

	// 既存のWidgetがある場合は表示/非表示を切り替え
	if (InventoryWidget)
	{
		if (InventoryWidget->IsInViewport())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hiding InventoryWidget"));
			InventoryWidget->RemoveFromParent();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Showing InventoryWidget again"));
			InventoryWidget->AddToViewport();
			InventoryWidget->SetInventory(InventoryComponent->Inventory);
		}
	}
}

void ATestHorrorGameCharacter::ToggleMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("ToggleMenu called!"));

	// MenuWidgetClassが設定されているか確認
	if (!MenuWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ MenuWidgetClass is not set in Blueprint!"));
		return;
	}

	// InventoryComponentの中身チェック
	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ InventoryComponent is null!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Menu: Inventory contains %d items"), InventoryComponent->Inventory.Num());

	// PlayerControllerとInputSubsystemを取得
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = nullptr;
	if (PlayerController)
	{
		Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	}

	// MenuWidgetがNull、かつMenuWidgetClassが設定されている場合は新しく作成
	if (!MenuWidget && MenuWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating new MenuWidget"));
		MenuWidget = CreateWidget<UMenuWidget>(GetWorld(), MenuWidgetClass);

		if (!MenuWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("❌ Failed to create MenuWidget!"));
			return;
		}

		// メニュー表示時の設定
		MenuWidget->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("✅ MenuWidget Added to Viewport (initial)"));
		MenuWidget->SetInventory(InventoryComponent->Inventory);

		// Input Mapping Contextをメニュー用に切り替え
		if (Subsystem && MenuMappingContext)
		{
			Subsystem->RemoveMappingContext(DefaultMappingContext);
			Subsystem->AddMappingContext(MenuMappingContext, 0);
			UE_LOG(LogTemp, Warning, TEXT("🎮 Switched to Menu Input Context"));
		}

		// マウスカーソルを表示
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeGameAndUI());
		}

		return;
	}

	// 既存のMenuWidgetがある場合は表示/非表示を切り替え
	if (MenuWidget)
	{
		if (MenuWidget->IsInViewport())
		{
			// メニューを閉じる
			UE_LOG(LogTemp, Warning, TEXT("Hiding MenuWidget"));
			MenuWidget->RemoveFromParent();

			// Input Mapping Contextを通常に戻す
			if (Subsystem && DefaultMappingContext)
			{
				Subsystem->RemoveMappingContext(MenuMappingContext);
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
				UE_LOG(LogTemp, Warning, TEXT("🎮 Switched back to Default Input Context"));
			}

			// マウスカーソルを非表示
			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = false;
				PlayerController->SetInputMode(FInputModeGameOnly());
			}

			// MenuWidgetの参照をクリア
			MenuWidget = nullptr;
		}
		else
		{
			// メニューを表示
			UE_LOG(LogTemp, Warning, TEXT("Showing MenuWidget again"));
			MenuWidget->AddToViewport();
			MenuWidget->SetInventory(InventoryComponent->Inventory);

			// Input Mapping Contextをメニュー用に切り替え
			if (Subsystem && MenuMappingContext)
			{
				Subsystem->RemoveMappingContext(DefaultMappingContext);
				Subsystem->AddMappingContext(MenuMappingContext, 0);
				UE_LOG(LogTemp, Warning, TEXT("🎮 Switched to Menu Input Context"));
			}

			// マウスカーソルを表示
			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(FInputModeGameAndUI());
			}
		}
	}
	else
	{
		// MenuWidgetがnullの場合（外部から削除された可能性）
		// Input Contextを確実に通常に戻す
		UE_LOG(LogTemp, Warning, TEXT("MenuWidget is null, ensuring default input context"));
		if (Subsystem && DefaultMappingContext)
		{
			Subsystem->RemoveMappingContext(MenuMappingContext);
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
			UE_LOG(LogTemp, Warning, TEXT("🎮 Restored Default Input Context"));
		}

		// マウスカーソルを非表示
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
		}
	}
}

void ATestHorrorGameCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("🔍 Interact called!"));
	UE_LOG(LogTemp, Warning, TEXT("📋 InteractableItems count: %d"), InteractableItems.Num());
	
	// インタラクト可能なアイテムがあるかチェック
	if (InteractableItems.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ No interactable items nearby"));
		return;
	}
	
	// 前方のアイテムを優先
	float ClosestDistance = FLT_MAX;
	AItemActor* ClosestItem = nullptr;
	
	FVector PlayerLocation = GetActorLocation();
	FVector PlayerForward = GetActorForwardVector();
	
	for (AItemActor* Item : InteractableItems)
	{
		if (!IsValid(Item) || !Item->bCanInteract)
		{
			continue;
		}
		
		FVector ToItem = Item->GetActorLocation() - PlayerLocation;
		float Distance = ToItem.Size();
		
		// 前方のアイテムを優先（内積を使用）
		ToItem.Normalize();
		float DotProduct = FVector::DotProduct(PlayerForward, ToItem);
		
		// 前方60度以内（cos(60°) = 0.5）で最も近いアイテムを選択
		if (DotProduct > 0.5f && Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestItem = Item;
		}
	}
	
	// 前方にアイテムがない場合は、最も近いアイテムを選択
	if (!ClosestItem && InteractableItems.Num() > 0)
	{
		for (AItemActor* Item : InteractableItems)
		{
			if (!IsValid(Item) || !Item->bCanInteract)
			{
				continue;
			}
			
			float Distance = FVector::Dist(Item->GetActorLocation(), PlayerLocation);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestItem = Item;
			}
		}
	}
	
	// 最も近いアイテムとインタラクト
	if (ClosestItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("📦 Interacting with item: %s"), *ClosestItem->ItemData.Name.ToString());
		ClosestItem->Interact(this);
		// 取得後はリストから削除
		InteractableItems.Remove(ClosestItem);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("❌ No valid items to interact with"));
	}
}
