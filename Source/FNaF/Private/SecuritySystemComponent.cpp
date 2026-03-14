// Fill out your copyright notice in the Description page of Project Settings.


#include "FNaF/Public/SecuritySystemComponent.h"

#include "PowerDrainComponent.h"

USecuritySystemComponent::USecuritySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USecuritySystemComponent::OpenMonitor(APlayerController* PC, AActor* OfficePawn)
{
	if (!PC || Cameras.IsEmpty()) return;
	
	if (const auto PowerDrainComponent = OfficePawn->FindComponentByClass<UPowerDrainComponent>())
	{
		PowerDrainComponent->StartUsingPower();
	}
	
	SwitchToCamera(PC, CurrentCameraIndex);
	UE_LOG(LogTemp, Log, TEXT("SecuritySystemComponent::Monitor opened"));
}

void USecuritySystemComponent::CloseMonitor(APlayerController* PC, AActor* OfficePawn)
{
	if (!PC || !OfficePawn) return;
	
	if (const auto PowerDrainComponent = OfficePawn->FindComponentByClass<UPowerDrainComponent>())
	{
		PowerDrainComponent->StopUsingPower();
	}

	PC->SetViewTarget(OfficePawn);
	UE_LOG(LogTemp, Log, TEXT("SecuritySystemComponent::Monitor closed"));
}

void USecuritySystemComponent::SwitchToCamera(APlayerController* PC, int32 CameraIndex)
{
	if (!PC || !Cameras.IsValidIndex(CameraIndex) || !Cameras[CameraIndex]) return;

	PC->SetViewTarget(Cameras[CameraIndex]);
	CurrentCameraIndex = CameraIndex;
}

