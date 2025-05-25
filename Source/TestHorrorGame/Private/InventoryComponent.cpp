// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::AddItem(const FItemData& NewItem, int32 Count)
{
	for (FInventorySlot& Slot : Inventory)
	{
		if (Slot.Item.ItemID == NewItem.ItemID && Slot.Count < NewItem.MaxStack)
		{
			Slot.Count = FMath::Min(Slot.Count + Count, NewItem.MaxStack);
			return true;
		}
	}

	FInventorySlot NewSlot;
	NewSlot.Item = NewItem;
	NewSlot.Count = Count;
	Inventory.Add(NewSlot);

	return true;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

