// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "MenuWidget.h"
#include "GameOverWidget.h"
#include "TestHorrorGameCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ATestHorrorGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Inventory Open/Close Input Action 
	* コンストラクタで初期設定されますが、Blueprintでオーバーライド可能です
	* デフォルトパス: /Game/Input/Actions/IA_OpenInventory
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenInventoryAction;

	/** Interact Input Action 
	* アイテムを拾うなどのインタラクション用
	* デフォルトパス: /Game/Input/Actions/IA_Interact
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

public:
	ATestHorrorGameCharacter();

	// インベントリコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	UInventoryComponent* InventoryComponent;

	// インベントリUIのクラス（Blueprintをアサイン）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	// 実体（AddToViewportされたもの）
	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	// メニューUIのクラス（Blueprintをアサイン）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UMenuWidget> MenuWidgetClass;

	// メニューウィジェットの実体
	UPROPERTY()
	UMenuWidget* MenuWidget;

	// ゲームオーバーUIのクラス（Blueprintをアサイン）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UGameOverWidget> GameOverWidgetClass;

	// ゲームオーバーウィジェットの実体
	UPROPERTY()
	UGameOverWidget* GameOverWidget;

	// 表示／非表示用関数
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	// メニュー表示／非表示用関数
	UFUNCTION(BlueprintCallable)
	void ToggleMenu();
	
	// インタラクト関数
	UFUNCTION(BlueprintCallable)
	void Interact();
	
	// 現在インタラクト可能なアイテム
	UPROPERTY()
	TArray<class AItemActor*> InteractableItems;

	// === ダメージシステム ===
	
	// プレイヤーの体力（現在は1で設定、攻撃を受けたら即死）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float Health;
	
	// 最大体力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float MaxHealth;
	
	// 死亡フラグ
	UPROPERTY(BlueprintReadOnly, Category="Health")
	bool bIsDead;
	
	// ダメージを受ける関数
	UFUNCTION(BlueprintCallable, Category="Health")
	void TakeDamage(float DamageAmount);
	
	// 死亡処理関数
	UFUNCTION(BlueprintCallable, Category="Health")
	void Die();
	
	// ゲームオーバー処理関数
	UFUNCTION(BlueprintCallable, Category="Health")
	void GameOver();

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	/** Menu MappingContext - メニュー表示中に使用 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* MenuMappingContext;
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
