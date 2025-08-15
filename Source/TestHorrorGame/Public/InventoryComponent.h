#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemData.h" // FItemDataが定義されているヘッダーを必ずインクルード

#include "InventoryComponent.generated.h"

// BlueprintTypeはここにも必要
USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FItemData Item;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Count;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTHORRORGAME_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:    
    // Sets default values for this component's properties
    UInventoryComponent();

    // インベントリの本体：スロットの配列
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory")
    TArray<FInventorySlot> Inventory;

    // アイテムを追加する関数
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool AddItem(const FItemData& NewItem, int32 Count = 1);

    // アイテムを持っているかチェックする関数
    UFUNCTION(BlueprintCallable, Category="Inventory")
    bool HasItem(const FName& ItemID) const;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};