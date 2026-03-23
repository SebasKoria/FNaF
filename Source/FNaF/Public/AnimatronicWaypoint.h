// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Identifiers.h"
#include "GameFramework/Actor.h"
#include "AnimatronicWaypoint.generated.h"


UCLASS()
class FNAF_API AAnimatronicWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, Category="Location")
	ERooms RoomTag;
	
	UPROPERTY(EditAnywhere, Category="Location")
	TArray<EAnimatronicID> AllowedAnimatronics;
};
