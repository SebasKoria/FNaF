// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimeSubsystem.generated.h"


class UPowerSubsystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHourChangedDelegate, int, Hour);

UCLASS()
class FNAF_API UTimeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FHourChangedDelegate OnHourChanged;

private:
	
	FTimerHandle TimerHandle;
	int CurrentHour = 0;
	
	UPROPERTY()
	UPowerSubsystem* CachedPowerSubsystem;
	
	UPROPERTY(EditAnywhere, Category = "TimeSubsystem")
	float HourDuration = 60.0f;
	
	void ChangeHour();
	
	UFUNCTION()
	void HandleBatteryDrained();
};
