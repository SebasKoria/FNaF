// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerDrainComponent.h"

#include "PowerSubsystem.h"


UPowerDrainComponent::UPowerDrainComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPowerDrainComponent::StartUsingPower()
{
	if (const UWorld* World = GetWorld())
	{
		if (UPowerSubsystem* PowerSubsystem = World->GetSubsystem<UPowerSubsystem>())
		{
			PowerSubsystem->IncreaseUsage();
		}
	}
}

void UPowerDrainComponent::StopUsingPower()
{
	if (const UWorld* World = GetWorld())
	{
		if (UPowerSubsystem* PowerSubsystem = World->GetSubsystem<UPowerSubsystem>())
		{
			PowerSubsystem->DecreaseUsage();
		}
	}
}

