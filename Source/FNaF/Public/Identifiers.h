#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAnimatronicID : uint8
{
	Freddy UMETA(DisplayName = "Freddy"),
	Bonnie UMETA(DisplayName = "Bonnie"),
	Chica UMETA(DisplayName = "Chica"),
	Foxy UMETA(DisplayName = "Foxy"),
};

UENUM(BlueprintType)
enum class ERooms : uint8
{
	ShowStage UMETA(DisplayName = "ShowStage"),
	DiningArea UMETA(DisplayName = "DiningArea"),
	PirateCove UMETA(DisplayName = "PirateCove"),
	WestHall UMETA(DisplayName = "WestHall"),
	WestHallCorner UMETA(DisplayName = "WestHallCorner"),
	SupplyCloset UMETA(DisplayName = "SupplyCloset"),
	EastHall UMETA(DisplayName = "EastHall"),
	EastHallCorner UMETA(DisplayName = "EastHallCorner"),
	Backstage UMETA(DisplayName = "Backstage"),
	Kitchen UMETA(DisplayName = "Kitchen"),
	Restrooms UMETA(DisplayName = "Restrooms"),
	WestDoor UMETA(DisplayName = "WestDoor"),
	EastDoor UMETA(DisplayName = "EastDoor"),
	Office UMETA(DisplayName = "Office"),
};