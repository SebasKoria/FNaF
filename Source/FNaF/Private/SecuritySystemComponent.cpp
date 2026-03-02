// Fill out your copyright notice in the Description page of Project Settings.


#include "FNaF/Public/SecuritySystemComponent.h"
#include "PowerManagerComponent.h"

USecuritySystemComponent::USecuritySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USecuritySystemComponent::OpenMonitor(APlayerController* PC, AActor* OfficePawn)
{
	if (!PC || Cameras.IsEmpty()) return;
	
	if (UPowerManagerComponent* PowerComp = OfficePawn->FindComponentByClass<UPowerManagerComponent>())
	{
		PowerComp->IncreaseUsage(); // ¡Sube el gasto de energía!
	}

	// Cambiar la vista a la última cámara seleccionada
	SwitchToCamera(PC, CurrentCameraIndex);

	// Aquí más adelante llamaremos a tu UI para mostrar la estática/mapa
	UE_LOG(LogTemp, Warning, TEXT("Monitor Abierto. Mostrando UI de cámaras."));
}

void USecuritySystemComponent::CloseMonitor(APlayerController* PC, AActor* OfficePawn)
{
	if (!PC || !OfficePawn) return;
	
	if (UPowerManagerComponent* PowerComp = OfficePawn->FindComponentByClass<UPowerManagerComponent>())
	{
		PowerComp->DecreaseUsage(); // ¡Baja el gasto de energía!
	}

	// Devolver la vista al jugador en la oficina
	PC->SetViewTarget(OfficePawn);

	// Aquí más adelante ocultaremos la UI del mapa
	UE_LOG(LogTemp, Warning, TEXT("Monitor Cerrado. De vuelta a la oficina."));
}

void USecuritySystemComponent::SwitchToCamera(APlayerController* PC, int32 CameraIndex)
{
	// Validar que el índice sea correcto
	if (!PC || !Cameras.IsValidIndex(CameraIndex) || !Cameras[CameraIndex]) return;

	// Cambiar la vista del jugador a esa cámara
	PC->SetViewTarget(Cameras[CameraIndex]);
	CurrentCameraIndex = CameraIndex;
}

