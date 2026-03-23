// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerSubsystem.h"
#include "TimeSubsystem.h"
#include "FNaF/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

void UPowerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	CachedPlayer = Cast<AMainCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	CurrentBattery = MaxBattery;
	CurrentUsageLevel = 1;
	bIsPowerDead = false;
	bIsGameOver = false;
	
	OnBatteryChanged.Broadcast(CurrentBattery);
	OnUsageChanged.Broadcast(CurrentUsageLevel);
	
	CachedTimeSubsystem = GetWorld()->GetSubsystem<UTimeSubsystem>();
	if (CachedTimeSubsystem)
	{
		CachedTimeSubsystem->OnHourChanged.AddDynamic(this, &UPowerSubsystem::HandleHourChanged);
	}
}

void UPowerSubsystem::Tick(float DeltaTime)
{
	if (bIsGameOver || bIsPowerDead) return;
	
	const float DecreaseAmount = BaseDrainRate * CurrentUsageLevel * DeltaTime;
	CurrentBattery -= DecreaseAmount;
	
	OnBatteryChanged.Broadcast(CurrentBattery);

	if (CurrentBattery <= 0.0f)
	{
		CurrentBattery = 0.0f;
		bIsPowerDead = true;
		CurrentUsageLevel = 0;
		
		OnBatteryChanged.Broadcast(CurrentBattery);
		OnUsageChanged.Broadcast(CurrentUsageLevel);
		OnBatteryDrained.Broadcast();
		
		if (CachedTimeSubsystem)
		{
			CachedTimeSubsystem->OnHourChanged.RemoveDynamic(this, &UPowerSubsystem::HandleHourChanged);
		}
	}
}

void UPowerSubsystem::IncreaseUsage()
{
	CurrentUsageLevel++;
	OnUsageChanged.Broadcast(CurrentUsageLevel);
}

void UPowerSubsystem::DecreaseUsage()
{
	CurrentUsageLevel--;
	OnUsageChanged.Broadcast(CurrentUsageLevel);
}

void UPowerSubsystem::HandleHourChanged(int NewHour)
{
	if (NewHour >= 6)
	{
		bIsGameOver = true;
		if (CachedTimeSubsystem)
		{
			CachedTimeSubsystem->OnHourChanged.RemoveDynamic(this, &UPowerSubsystem::HandleHourChanged);
		}
	}
}

TStatId UPowerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UPowerSubsystem, STATGROUP_Tickables);
}

bool UPowerSubsystem::IsTickable() const
{
	return FTickableGameObject::IsTickable();
}
