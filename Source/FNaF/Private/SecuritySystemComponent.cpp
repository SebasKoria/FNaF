// Fill out your copyright notice in the Description page of Project Settings.


#include "FNaF/Public/SecuritySystemComponent.h"

#include "PowerDrainComponent.h"

USecuritySystemComponent::USecuritySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USecuritySystemComponent::OpenMonitor(APlayerController* PC, AActor* OfficePawn)
{
	if (bIsMonitorOpen) return;
	if (!PC || Cameras.IsEmpty()) return;
	
	bIsMonitorOpen = true;
	
	if (const auto PowerDrainComponent = OfficePawn->FindComponentByClass<UPowerDrainComponent>())
	{
		PowerDrainComponent->StartUsingPower();
	}
	
	SwitchToCamera(PC, CurrentCameraIndex);
}

void USecuritySystemComponent::CloseMonitor(APlayerController* PC, AActor* OfficePawn)
{
	if (!bIsMonitorOpen) return;
	if (!PC || !OfficePawn) return;
	
	bIsMonitorOpen = false;
	
	if (const auto PowerDrainComponent = OfficePawn->FindComponentByClass<UPowerDrainComponent>())
	{
		PowerDrainComponent->StopUsingPower();
	}

	PC->SetViewTarget(OfficePawn);
}

void USecuritySystemComponent::SwitchToCamera(APlayerController* PC, int32 CameraIndex)
{
	if (!PC || !Cameras.IsValidIndex(CameraIndex) || !Cameras[CameraIndex]) return;

	PC->SetViewTarget(Cameras[CameraIndex]);
	CurrentCameraIndex = CameraIndex;
}

