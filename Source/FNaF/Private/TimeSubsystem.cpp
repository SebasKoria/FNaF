// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeSubsystem.h"
#include "PowerSubsystem.h"

void UTimeSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
  Super::OnWorldBeginPlay(InWorld);
  
  CurrentHour = 0;
  OnHourChanged.Broadcast(CurrentHour);
  
  GetWorld()->GetTimerManager().SetTimer(
    TimerHandle,
    this,
    &UTimeSubsystem::ChangeHour,
    HourDuration,
    true
    );
  
  CachedPowerSubsystem = GetWorld()->GetSubsystem<UPowerSubsystem>();
  if (CachedPowerSubsystem)
  {
    CachedPowerSubsystem->OnBatteryDrained.AddDynamic(this, &UTimeSubsystem::HandleBatteryDrained);
  }
}

void UTimeSubsystem::ChangeHour()
{
  CurrentHour++;
  OnHourChanged.Broadcast(CurrentHour);
  
  if (CurrentHour >= 6)
  {
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    if (CachedPowerSubsystem)
    {
      CachedPowerSubsystem->OnBatteryDrained.RemoveDynamic(this, &UTimeSubsystem::HandleBatteryDrained);
    }
  }
}

void UTimeSubsystem::HandleBatteryDrained()
{
  GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
  if (CachedPowerSubsystem)
  {
    CachedPowerSubsystem->OnBatteryDrained.RemoveDynamic(this, &UTimeSubsystem::HandleBatteryDrained);
  }
}


